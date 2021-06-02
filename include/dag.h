//
// Created by 염준영 on 2021/06/02.
//

#ifndef SUBGRAPH_MATCHING_DAG_H
#define SUBGRAPH_MATCHING_DAG_H

#include <queue>
#include "graph.h"
#include "common.h"
#include <limits>
class DAG {
public:
    DAG(const Graph& query, const Graph& data)
    {
    }

    ~DAG() {

    }

private:
    void BFS(const Graph& query, const Graph& data)
    {
      std::queue<Vertex> q;
      Vertex r = findRoot(query, data);
      q.push(r);
      // add root to dag
      while (!q.empty())
      {
        Vertex v = q.front();
        int start = query.GetNeighborStartOffset(v);
        int end = query.GetNeighborEndOffset(v);
        for (int i = start; i < end; ++i)
        {
          // for Neighbors
          // add to dag its edges
          // push them to queue

          q.push(query.GetNeighbor(i));
        }
      }
    }
    // r <- argmin_{u in V(q)} |C_{ini}(u)|/deg_q(u)
    int findRoot(const Graph& query, const Graph& data)
    {
      int qsize = query.GetNumVertices();
      int dsize = data.GetNumVertices();
      int minIndex = 0;
      double min =  numeric_limits<double>::max();
      for (int i = 0; i < qsize; ++i) {
        int C_ini = 0;
        int deg = query.GetDegree(i);
        int label = query.GetLabel(i);
        for (int j = 0; j < dsize; ++j)
        {
          if (data.GetLabel(j) == label && data.GetDegree(j) >= deg)
            C_ini ++;
        }
        if (min > (float) (C_ini) / query.GetDegree(i))
        {
          minIndex = i;
          min = (float ) (C_ini) / query.GetDegree(i);
        }
      }
      return minIndex;
    }

    std::vector<std::vector<Vertex>> dag;
};

#endif //SUBGRAPH_MATCHING_DAG_H
