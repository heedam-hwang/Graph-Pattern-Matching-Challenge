/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include "dag.h"
#include <chrono>

#define MAX_SIZE 100000
template<typename T>
using riter = typename std::vector<T>::reverse_iterator;

Backtrack::Backtrack() {}

Backtrack::~Backtrack() {}

int Backtrack::count;
int Backtrack::count_error;

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  int num = query.GetNumVertices();
  std::vector<int> ans(num, -1);
  std::cout << "t " << num << "\n";


  Backtrack::count = 0;
  // std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  DAG dag(query, data);
  dag.InitWeight(cs, data);
  AdaptiveMatching(data, query, cs, 0, num, ans, dag);
  // std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
  // std::chrono::duration<double> time = end - start;
  // std::cout << "Adaptive Matching Elapsed Time: " << time.count() << "s\n";
  // std::cout << "Adaptive Matching Correct Rate: " << Backtrack::count - Backtrack::count_error << "/"
  //           << Backtrack::count << "\n";

}

void Backtrack::PrintCandidates(std::vector<int> &ans) {
  std::cout << "a";
  for (int i: ans) {
    std::cout << " " << i;
  }
  std::cout << "\n";
}

bool Backtrack::AdaptiveMatching(const Graph &data, const Graph &query, const CandidateSet &cs, int index, int size,
                                 std::vector<int> &acc, const DAG &dag) {
  if (index == size) {
    PrintCandidates(acc);
    Backtrack::count++;
    // if (!checkAnswer(acc, data, query)) {
    //   Backtrack::count_error++;
    // }
    if (count == MAX_SIZE)
      return true;

    return false;
  } else if (index == 0) {
    int root = dag.getRoot();
    int csNum = cs.GetCandidateSize(root);
    for (int _i = 0; _i < csNum; ++_i) {
      int ithCandidate = cs.GetCandidate(root, _i);
      acc[root] = ithCandidate;
      bool isEnd = AdaptiveMatching(data, query, cs, index + 1, size, acc, dag);
      acc[root] = -1;
      if (isEnd)
        return true;
    }
  } else {
    int nextV = dag.nextV(acc, data, cs);
    if (nextV == -1) {
      return false;
    }
    std::vector<Vertex> extendable = dag.extendable(acc, nextV, cs, data);
    std::sort(extendable.begin(), extendable.end(),
              [&](Vertex a, Vertex b) -> bool {
                  int n = query.GetNumLabels();
                  int max = query.GetNeighborLabelFrequency(nextV, 0);
                  int maxIdx = 0;
                  for (int i = 0; i < n; ++i) {
                    int temp = query.GetNeighborLabelFrequency(nextV, i);
                    if (temp > max) {
                      maxIdx = i;
                      max = temp;
                    }
                  }

                  int tempA = data.GetNeighborLabelFrequency(a, maxIdx);
                  int tempB = data.GetNeighborLabelFrequency(b, maxIdx);
                  return tempA > tempB;
              }
    );
    for (Vertex v: extendable) {
      if (std::find(acc.begin(), acc.end(), v) != acc.end()) {
        continue;
      }
      acc[nextV] = v;
      bool isEnd = AdaptiveMatching(data, query, cs, index + 1, size, acc, dag);
      acc[nextV] = -1;
      if (isEnd)
        return true;
    }
  }
  return false;
}


bool Backtrack::checkAnswer(std::vector<int> &acc, const Graph &data, const Graph &query) {
  bool ans = true;
  for (int i = 0; i < query.GetNumVertices(); i++) {
    /* 먼저 vertex label이 같은지 체크 */
    if (query.GetLabel(i) != data.GetLabel(acc[i])) {
      std::cout << "========== " << i << "th vertex label does not match! ========== " << std::endl;
      ans = false;
      return ans;
    }
    /* data graph와 query graph 사이의 edge 관계가 같은지 체크: query graph에 edge가 있다면 data graph에도 edge가 있어야*/
    for (int j = i + 1; j < query.GetNumVertices(); j++) {
      if (query.IsNeighbor(i, j) && !data.IsNeighbor(acc[i], acc[j])) {
        std::cout << "========== " << acc[i] << " and " << acc[j]
                  << " does not have edge on data graph ========== "
                  << std::endl;
        ans = false;
        return ans;
      }
    }
  }

  for (int i = 0; i < acc.size(); ++i)
  {
    for (int j = i + 1; j < acc.size(); ++j)
    {
      // 같은 Vertex가 두번 골라졌는지 Check
      if (acc[i] == acc[j])
        return false;
    }
  }
  return ans;
}

