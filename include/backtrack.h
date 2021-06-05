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

    void PrintCandidates(std::vector<int> &ans);

    bool checkAnswer(std::vector<int> &acc, const Graph &data, const Graph &query);

    bool AdaptiveMatching(const Graph &data, const Graph &query, const CandidateSet &cs,
                          int index, int size, std::vector<int> &acc, const DAG &dag);

};

#endif  // BACKTRACK_H_
