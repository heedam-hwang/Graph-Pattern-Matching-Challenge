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
    DAG(const Graph &query, const Graph &data);

    bool isEdge(Vertex i, Vertex j);

    int getSize();

    Vertex getRoot();

    void PrintDAG();


private:
    void BFS(const Graph &query, const Graph &data);

    void Weight();

    int findRoot(const Graph &query, const Graph &data);

    int size;
    int edge;
    Vertex root;

    std::vector<std::vector<Vertex>> dag;
    std::vector<Vertex> bfs_order;
    std::vector<int> weight;
};

#endif //SUBGRAPH_MATCHING_DAG_H
