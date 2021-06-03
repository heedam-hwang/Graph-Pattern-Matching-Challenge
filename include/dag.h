//
// Created by 염준영 on 2021/06/02.
//

#ifndef SUBGRAPH_MATCHING_DAG_H
#define SUBGRAPH_MATCHING_DAG_H

#include <queue>
#include "graph.h"
#include "common.h"
#include "candidate_set.h"
#include <limits>

class DAG {
public:
    DAG(const Graph &query, const Graph &data);

    bool isEdge(Vertex i, Vertex j);

    int getSize();

    Vertex getRoot();

    void PrintDAG();

    void InitWeight(const CandidateSet& cs, const Graph& data);


private:
    void BFS(const Graph &query, const Graph &data);


    int findRoot(const Graph &query, const Graph &data);

    int size;
    int edge;
    Vertex root;
// Weight array를 만들기 위해 parent 수 미리 기록
    std::vector<std::vector<Vertex>> dag;
    std::vector<int> parents;
    std::vector<Vertex> bfs_order;
};

#endif //SUBGRAPH_MATCHING_DAG_H
