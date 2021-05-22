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
    static int count;
    static int count_error;
    void NaiveCheck(const Graph &data, const Graph &query, const CandidateSet &cs,
                    int index, int size, std::vector<int> &acc);

    void PrintCandidates(std::vector<int> &ans);

    bool CheckNeighbors(const Graph &data, const Graph &query, const CandidateSet &cs,
                        int index, int csIndex);

    bool checkAnswer(std::vector<int> &acc, const Graph &data, const Graph &query);
};

#endif  // BACKTRACK_H_
