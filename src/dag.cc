//
// Created by zooon0 on 6/2/21.
//

#include "dag.h"
#include <iostream>

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
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      // from bfs_order[i] to bfs_order[j]
      if (query.IsNeighbor(bfs_order[i], bfs_order[j])) {
        dag[bfs_order[i]][bfs_order[j]] = 1;
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
/*
  DAG 잘만들었는지 체크용
  if (count != edge)
    std::cout << "DAG ERROR\n";
  std::cout << "Number of edges: " << edge << "\n";
*/
}

Vertex DAG::getRoot() {
  return root;
}

void DAG::InitWeight(const CandidateSet &cs, const Graph& data) {
  std::vector<std::vector<int>> w(size);

  for (int u = 0; u < size; ++u)
  {
    std::vector<Vertex> childs;
    int n = cs.GetCandidateSize(u);
    w[u].resize(n, 0);
    int k = dag[u].size();

    for (int j = 0; j < k; ++j)
    {
      if (dag[u][j] == 1)
        childs.push_back(j);
    }

    if (childs.empty())
      std::find(childs.begin(), childs.end(), 1);
    // w[u][j] <- W_{u}(v)
    else
    {
      k = childs.size();
      // c_i = childs[i]
      // 각 v(=j) 마다
      int min = INT_MAX;
      for (int v = 0; v < n; ++v)
      {
        int temp;
        for (int i = 0; i < k; ++i)
        {
          temp = 0;
          int csSize = cs.GetCandidateSize(childs[i]);

          // W_{u, c_i} (v) 계산
          for (int vn = 0; vn < csSize; ++vn)
          {
            if (data.IsNeighbor(cs.GetCandidate(childs[i], vn), cs.GetCandidate(u, v)))
            {
              temp += w[childs[i]][cs.GetCandidate(childs[i], vn)];
            }
          }
          if (min > temp)
            min = temp;
        }
        w[u][v] = min;
      }
    }

  }

}
