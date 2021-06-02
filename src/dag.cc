//
// Created by zooon0 on 6/2/21.
//

#include "dag.h"
#include <iostream>

DAG::DAG(const Graph &query, const Graph &data) {
  int n = query.GetNumVertices();
  std::vector<Vertex> temp (n, 0);
  dag.resize(n, temp);
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
  std::vector<Vertex> bfs_order;
  Vertex r = findRoot(query, data);
  q.push(r);
  bfs_order.push_back(r);

  while (!q.empty()) {
    Vertex v = q.front();
    q.pop();
    int start = query.GetNeighborStartOffset(v);
    int end = query.GetNeighborEndOffset(v);
    for (int i = start; i < end; ++i) {
      int n = query.GetNeighbor(i);

      // check that the node is marked
      if (std::find(bfs_order.begin(), bfs_order.end(), n) == bfs_order.end())
      {
        bfs_order.push_back(n);
        q.push(n);
      }

    }
  }

  int n = bfs_order.size();
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j)
    {
      if (query.IsNeighbor(bfs_order[i], bfs_order[j]))
      {
        dag[bfs_order[i]][bfs_order[j]] = 1;
        edge++;
      }
    }
  }

}

void DAG::PrintDAG() {
  for (int i = 0; i < size; ++i)
    for (int j = 0; j < size; ++j)
    {
      if (dag[i][j] == 1)
      {
        std::cout << "Edge from " << i << " to " << j << "\n";
        count++;
      }
    }
}
