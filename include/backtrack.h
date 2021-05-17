/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"

class Backtrack {
public:
    Backtrack();

    ~Backtrack();

    void PrintAllMatches(const Graph &data, const Graph &query,
                         const CandidateSet &cs);

private:
    void NaiveCheck(const Graph &data, const Graph &query, const CandidateSet &cs,
                    int index, int size, std::vector<int> &acc);

    void PrintCandidates(std::vector<int> &ans);

    bool CheckNeighbors(const Graph &data, const Graph &query, const CandidateSet &cs,
                        int index, int csIndex);

    void checkAnswer(std::vector<int> &acc, const Graph &data, const Graph &query);
};

#endif  // BACKTRACK_H_
