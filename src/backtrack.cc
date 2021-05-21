/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <chrono>

Backtrack::Backtrack() {}

Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  int num = query.GetNumVertices();
  std::vector<int> ans(num, -1);
  std::cout << "t " << num << "\n";
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  NaiveCheck(data, query, cs, 0, num, ans);
  std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
  std::chrono::duration<double> time = end - start;
  std::cout << "Elapsed Time: " << time.count() << "s\n";
}

void Backtrack::PrintCandidates(std::vector<int> &ans) {
  std::cout << "a";
  for (int i: ans) {
    std::cout << " " << i;
  }
  std::cout << "\n";
}

bool Backtrack::CheckNeighbors(const Graph &data, const Graph &query, const CandidateSet &cs, int index, int csIndex) {
  // index: u_i, csIndex: v_j = C(u_i)[_i]
  // temp: if (v_j has kth Neighbor's label of u_i)
  // for ni in N(query(index)) there exists a nic in N(data(csIndex)) -> true else false
  bool ans = true;
  bool temp = false;
  int queryNbhStart = query.GetNeighborStartOffset(index);
  int queryNbhEnd = query.GetNeighborEndOffset(index);
//  int csNbhStart = data.GetNeighborStartOffset(csIndex);
//  int csNbhEnd = data.GetNeighborEndOffset(csIndex);
  for (int _i = queryNbhStart; _i < queryNbhEnd; ++_i) {
    temp = false;
//    int queryNbhLabel = query.GetLabel(query.GetNeighbor(_i));
//    for (int _j = csNbhStart; _j < csNbhEnd; ++_j) {
//      if (queryNbhLabel == data.GetLabel(data.GetNeighbor(_j))) {
//        temp = true;
//        break;
//      }
//    }
// query의 이웃의 Candidate만 확인하면 됨
// 개선방안: Dynamic Programming으로 이전에 확인했을때 실패한 노드는 확인안하기
// (성공한 노드를 배열에 저장해놓고 그 안에서만 확인, 매번 업데이트)
// (+ Neighbor에 대해서도 비슷한 방법 사용가능할듯)
    int ithNbh = query.GetNeighbor(_i);
    int queryNbhCsSize = cs.GetCandidateSize(ithNbh);
    for (int _j = 0; _j < queryNbhCsSize; ++_j) {
      if (data.IsNeighbor(csIndex, cs.GetCandidate(ithNbh, _j))) {
        temp = true;
        break;
      }
    }
    if (!temp)
      return false;
  }
  return true;
}

// Graph에 label frequency를 구할 수 있는 함수가 있음, 이걸로 좀 더 효율적인 검색을 할 수 있을듯?
void Backtrack::NaiveCheck(const Graph &data, const Graph &query, const CandidateSet &cs,
                           int index, int size, std::vector<int> &acc) {
  if (index == size) {
    PrintCandidates(acc);
    /* 제출 시 checkAnswer는 comment out */
//    checkAnswer(acc, data, query);
    return;
  } else {
    int csNum = cs.GetCandidateSize(index);
    for (int _i = 0; _i < csNum; ++_i) {
      /*
       * 우선 현재 고른 노드가 이전에 골라지지 않았는지 확인(하나의 노드가 C(u), C(v)에 둘다 들어갈 가능성)
       * 따라서 매번 재귀호출후 acc[index] = -1로 다시 초기화
       * 맞으면, query의 이웃들이 고른 노드의 이웃에 포함되는지 확인
       * begin부터 end까지 찾지말고 begin부터 acc[index-1]까지만 찾아도 될듯
       */
      int ithCandidate = cs.GetCandidate(index, _i);
      if (std::find(acc.begin(), acc.end(), ithCandidate) == acc.end() &&
          CheckNeighbors(data, query, cs, index, ithCandidate)) {
        acc[index] = ithCandidate;
        NaiveCheck(data, query, cs, index + 1, size, acc);
        acc[index] = -1;
      }
    }
    return;
  }
}

void Backtrack::checkAnswer(std::vector<int> &acc, const Graph &data, const Graph &query) {
  for (int i = 0; i < query.GetNumVertices(); i++) {
    /* 먼저 vertext label이 같은지 체크 */
    if (query.GetLabel(i) != data.GetLabel(acc[i])) {
      std::cout << "========== " << i << "th vertex label does not match! ========== " << std::endl;
    }
    /* data graph와 query graph 사이의 edge 관계가 같은지 체크: query graph에 edge가 있다면 data graph에도 edge가 있어야*/
    for (int j = i + 1; j < query.GetNumVertices(); j++) {
      if (query.IsNeighbor(i, j) && !data.IsNeighbor(acc[i], acc[j])) {
        std::cout << "========== " << acc[i] << " and " << acc[j] << " does not have edge on data graph ========== "
                  << std::endl;
      }
    }
  }
}