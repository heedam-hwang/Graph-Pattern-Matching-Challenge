//
// Created by zooon0 on 6/2/21.
//

#include "dag.h"
#include <iostream>

template<typename T>
using riter = typename std::vector<T>::reverse_iterator;

DAG::DAG(const Graph &query, const Graph &data) {
  int n = query.GetNumVertices();
  std::vector<Vertex> temp(n, 0);
  dag.resize(n, temp);
  parents.resize(n, 0);
  size = n;
  edge = 0;
  BFS(query, data);
}

int DAG::getSize() {
  return size;
}

bool DAG::isEdge(Vertex i, Vertex j) {
  return (dag[i][j] == 1);
}

// r <- argmin_{u in V(q)} |C_{ini}(u)|/deg_q(u)
int DAG::findRoot(const Graph &query, const Graph &data) {
  int qsize = query.GetNumVertices();
  int dsize = data.GetNumVertices();
  int minIndex = 0;
  double min = std::numeric_limits<double>::max();
  for (int i = 0; i < qsize; ++i) {
    int C_ini = 0;
    int deg = query.GetDegree(i);
    int label = query.GetLabel(i);
    for (int j = 0; j < dsize; ++j) {
      if (data.GetLabel(j) == label && data.GetDegree(j) >= deg)
        C_ini++;
    }
    if (min > (float) (C_ini) / query.GetDegree(i)) {
      minIndex = i;
      min = (float) (C_ini) / query.GetDegree(i);
    }
  }
  return minIndex;
}

void DAG::BFS(const Graph &query, const Graph &data) {
  std::queue<Vertex> q;
  Vertex r = findRoot(query, data);
  q.push(r);
  bfs_order.push_back(r);
  root = r;

  while (!q.empty()) {
    Vertex v = q.front();
    q.pop();
    int start = query.GetNeighborStartOffset(v);
    int end = query.GetNeighborEndOffset(v);
    for (int i = start; i < end; ++i) {
      int n = query.GetNeighbor(i);

      // check that the node is marked
      if (std::find(bfs_order.begin(), bfs_order.end(), n) == bfs_order.end()) {
        bfs_order.push_back(n);
        q.push(n);
      }

    }
  }

  int n = bfs_order.size();
  parent_list.resize(n);
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      // from bfs_order[i] to bfs_order[j]
      if (query.IsNeighbor(bfs_order[i], bfs_order[j])) {
        dag[bfs_order[i]][bfs_order[j]] = 1;
        parent_list[bfs_order[j]].push_back(bfs_order[i]);
        parents[bfs_order[j]]++;
        edge++;
      }
    }
  }

}

void DAG::PrintDAG() {
//  int count = 0;
  for (int i = 0; i < size; ++i)
    for (int j = 0; j < size; ++j) {
      if (dag[i][j] == 1) {
        std::cout << "Edge from " << i << " to " << j << "\n";
//        count++;
      }
    }
  std::cout << "BFS ORDER\n";
  for (Vertex v: bfs_order)
    std::cout << v << " ";
  std::cout << "\n";
/*
  DAG 잘만들었는지 체크용
  if (count != edge)
    std::cout << "DAG ERROR\n";
  std::cout << "Number of edges: " << edge << "\n";
*/
}

Vertex DAG::getRoot() const {
  return root;
}

void DAG::InitWeight(const CandidateSet &cs, const Graph &data) {
  w.resize(size);

  for (riter<Vertex> iter = bfs_order.rbegin(); iter != bfs_order.rend(); ++iter) {
    Vertex u = *iter;
    int s = cs.GetCandidateSize(u);
    std::vector<Vertex> childs;
    w[u].resize(s, 0);

    // u -> i
    int u_size = dag[u].size();
    for (int i = 0; i < u_size; ++i) {
      if (parents[i] == 1 && dag[u][i] == 1)
        childs.push_back(i);
    }
    // childs = C_i

    if (childs.empty()) {
      std::fill(w[u].begin(), w[u].end(), 1);
    } else {
      // v in C(u)
      int v_idx = cs.GetCandidateSize(u);

      // W[u][v] 계산
      for (int j = 0; j < v_idx; ++j) {
        Vertex v = cs.GetCandidate(u, j);

        int min = INT_MAX;
        for (Vertex c_i: childs) {
          int temp_weight = 0;
          int vp_idx = cs.GetCandidateSize(c_i);

          for (int k = 0; k < vp_idx; ++k) {
            Vertex vprime = cs.GetCandidate(c_i, k);
            if (data.IsNeighbor(vprime, v)) {
              temp_weight += w[c_i][k];
            }
          }

          // temp_weight = W_{u, c_i}(v)
          if (min > temp_weight) {
            min = temp_weight;
          }
        }
        w[u][j] = min;
      }
    }

  }
  weight.resize(size, 0);

  for (int i = 0; i < size; ++i) {
    for (int j : w[i]) {
      weight[i] += j;
    }
  }

}

void DAG::PrintWeight() {
//  for (int i = 0; i < size; ++i)
//  {
//    int k = w[i].size();
//    for (int j = 0; j < k; ++j) {
//      std::cout << "u: " << i << ", v: " << j << " weight: " << w[i][j] << "\n";
//    }
//  }
  for (int i = 0; i < size; ++i) {
    std::cout << "u" << i << ": " << weight[i] << "\n";
  }
}

Vertex DAG::nextV(std::vector<Vertex> &acc, const Graph& data, const CandidateSet& cs) const {
  // acc: matched list
  int min = INT_MAX;
  Vertex minIdx = -1;
  for (int i = 0; i < size; ++i) {
    if (acc[i] != -1)
      continue;
    bool allmatched = true;
    for (Vertex v: parent_list[i]) {
      if (acc[v] == -1) {
        allmatched = false;
        break;
      }
    }
    if (allmatched)
    {
      int tempweight = 0;
//      std::vector<Vertex> C_M_u = extendable(acc, i, cs, data);

//      for (Vertex v: C_M_u)
//      {
//        tempweight += w[i][v];
//      }
      // C_M_u.size() 를 쓰면 Candidate Size order
//       tempweight -=  C_M_u.size();
//       tempweight *= tempweight;
      tempweight += weight[i];

      if (min > tempweight) {
        min = tempweight;
        minIdx = i;
      }
    }
  }
  return minIdx;
}

std::vector<Vertex>
DAG::extendable(const std::vector<Vertex> &acc, Vertex u, const CandidateSet &cs, const Graph &data) const {
  // M(p_i) 에 adjacent, v in C(u)
  std::vector<Vertex> ans;

  int num = cs.GetCandidateSize(u);

  for (int i = 0; i < num; ++i) {
    Vertex v = cs.GetCandidate(u, i);
    bool extendable = true;

    for (Vertex k: parent_list[u]) {
      if (acc[k] == -1) {
        extendable = false;
        break;
      } else if (!data.IsNeighbor(acc[k], v)) {
        extendable = false;
        break;
      } else {
        continue;
      }
    }
    if (extendable)
      ans.push_back(v);
  }

  return ans;
}
