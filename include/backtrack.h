/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include "dag.h"

class Backtrack {
public:
    Backtrack();

    ~Backtrack();

    void PrintAllMatches(const Graph &data, const Graph &query,
                         const CandidateSet &cs);

private:
    static int count;
    static int count_error;

    bool NaiveCheck(const Graph &data, const Graph &query, const CandidateSet &cs,
                    int index, int size, std::vector<int> &acc);

    void PrintCandidates(std::vector<int> &ans);

    bool CheckNeighbors(const Graph &data, const Graph &query, const CandidateSet &cs,
                        int index, int csIndex);

    bool checkAnswer(std::vector<int> &acc, const Graph &data, const Graph &query);

    bool CheckNeighborsWithDP(const Graph &data, const Graph &query, const CandidateSet &cs,
                              int index, int csIndex, std::vector<std::vector<Vertex>> &cs_dp,
                              std::vector<std::vector<Vertex>> &cs_dp_next);

    bool CheckWithDP(const Graph &data, const Graph &query, const CandidateSet &cs,
                     int index, int size, std::vector<int> &acc, std::vector<std::vector<Vertex>> &cs_dp);

    bool AdaptiveMatching(const Graph &data, const Graph &query, const CandidateSet &cs,
                         int index, int size, std::vector<int> &acc, const DAG &dag);

};

#endif  // BACKTRACK_H_
