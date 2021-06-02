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
    DAG(const Graph& query, const Graph& data);
    bool isEdge(Vertex i, Vertex j);
    int getSize();

    void PrintDAG();


private:
    void BFS(const Graph& query, const Graph& data);
    int findRoot(const Graph& query, const Graph& data);
    int size;
    int edge;

    std::vector<std::vector<Vertex>> dag;
};

#endif //SUBGRAPH_MATCHING_DAG_H
