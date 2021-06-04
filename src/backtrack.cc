/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include "dag.h"
#include <chrono>

#define MAX_SIZE 100000

Backtrack::Backtrack() {}

Backtrack::~Backtrack() {}

int Backtrack::count;
int Backtrack::count_error;

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  int num = query.GetNumVertices();
  std::vector<int> ans(num, -1);
  std::cout << "t " << num << "\n";


  Backtrack::count = Backtrack::count_error = 0;
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  DAG dag(query, data);
  dag.InitWeight(cs, data);
  AdaptiveMatching(data, query, cs, 0, num, ans, dag);
  std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
  std::chrono::duration<double> time = end - start;
  std::cout << "Adaptive Matching Elapsed Time: " << time.count() << "s\n";
  std::cout << "Adaptive Matching Correct Rate: " << Backtrack::count - Backtrack::count_error << "/" << Backtrack::count << "\n";


//  Backtrack::count = Backtrack::count_error = 0;
//  start = std::chrono::system_clock::now();
//  NaiveCheck(data, query, cs, 0, num , ans);
//  end = std::chrono::system_clock::now();
//  time = end - start;
//  std::cout << "Naive Check Elapsed Time: " << time.count() << "s\n";
//  std::cout << "Naive Check Correct Rate: " << Backtrack::count - Backtrack::count_error << "/" << Backtrack::count << "\n";
//
//  Backtrack::count = Backtrack::count_error = 0;
//  start = std::chrono::system_clock::now();
//  std::vector<Vertex> _a;
//  std::vector<std::vector<Vertex>> __a(num, _a);
//  CheckWithDP(data, query, cs, 0, num, ans, __a);
//  end = std::chrono::system_clock::now();
//  time = end -start;
//  std::cout << "DP Check Elapsed Time: " << time.count() << "s\n";
//  std::cout << "DP Check Correct Rate: " << Backtrack::count - Backtrack::count_error << "/" << Backtrack::count << "\n";

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
    if (!checkAnswer(acc, data, query)) {
      Backtrack::count_error++;
    }
    if (count == MAX_SIZE)
      return true;

    return false;
  } else if (index == 0) {
    int root = dag.getRoot();
    int csNum = cs.GetCandidateSize(root);
    for (int _i = 0; _i < csNum; ++_i)
    {
      int ithCandidate = cs.GetCandidate(root, _i);
      acc[root] = ithCandidate;
      bool isEnd = AdaptiveMatching(data, query, cs, index + 1, size, acc, dag);
      acc[root] = -1;
      if (isEnd)
        return true;
    }
  }
  else {
    int nextV = dag.nextV(acc, data, cs);
    if (nextV == -1) {
      return false;
    }
    std::vector<Vertex> extendable = dag.extendable(acc, nextV, cs, data);
    for (Vertex v: extendable)
    {
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
bool Backtrack::NaiveCheck(const Graph &data, const Graph &query, const CandidateSet &cs,
                           int index, int size, std::vector<int> &acc) {
  if (index == size) {
    // 극단적인 수단: checkAnswer해서 맞는 거만 출력
    /* 제출 시 checkAnswer는 comment out */
    PrintCandidates(acc);
    Backtrack::count++;
    if (!checkAnswer(acc, data, query)) {
      Backtrack::count_error++;
    }
    if (count == MAX_SIZE)
      return true;

    return false;
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
        bool isEnd = NaiveCheck(data, query, cs, index + 1, size, acc);
        acc[index] = -1;
        if (isEnd)
          return true;
      }
    }
    return false;
  }
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
  return ans;
}

bool Backtrack::CheckNeighborsWithDP(const Graph &data, const Graph &query, const CandidateSet &cs, int index,
                                     int csIndex, std::vector<std::vector<Vertex>> &cs_dp,
                                     std::vector<std::vector<Vertex>> &cs_dp_next) {
  bool ans = true;
  bool temp = false;
  int queryNbhStart = query.GetNeighborStartOffset(index);
  int queryNbhEnd = query.GetNeighborEndOffset(index);
  if (cs_dp_next.empty()) {
    std::vector<Vertex> _a;
    for (int _i = 0; _i < query.GetNumVertices(); ++_i) {
      cs_dp_next.push_back(_a);
    }
    for (int _i = queryNbhStart; _i < queryNbhEnd; ++_i) {
      temp = false;
      int ithNbh = query.GetNeighbor(_i);
      int queryNbhCsSize = cs.GetCandidateSize(ithNbh);
      for (int _j = 0; _j < queryNbhCsSize; ++_j) {
        if (data.IsNeighbor(cs.GetCandidate(index, csIndex), cs.GetCandidate(ithNbh, _j))) {
          temp = true;
          cs_dp_next[ithNbh].push_back(cs.GetCandidate(ithNbh, _j));
        }
      }
      if (!temp)
        ans = false;
      else {
        cs_dp_next[csIndex].push_back(_i);
      }
    }
  } else {
    for (int _i = 0; _i < queryNbhEnd - queryNbhStart; ++_i) {
      temp = false;
      int ithNbh = query.GetNeighbor(_i + queryNbhStart);
      int queryNbhCsSize = cs_dp_next[ithNbh].size();
      for (int _j = 0; _j < queryNbhCsSize; ++_j) {
        if (data.IsNeighbor(cs.GetCandidate(index, csIndex), cs_dp_next[ithNbh][_j])) {
          temp = true;
          break;
        }
      }
      if (!temp)
        return false;
    }
  }
  if (cs_dp[csIndex].empty()) {
    cs_dp.assign(cs_dp_next.begin(), cs_dp_next.end());
  }
  return ans;
}

// cs_dp: candidate set의 멤버 중 가능한 것들(CheckNeighborWithDP에서 확인된 것들)
// CheckNeighborWithDP: check Neighbors and fill cs_dp_next
bool Backtrack::CheckWithDP(const Graph &data, const Graph &query, const CandidateSet &cs, int index, int size,
                            std::vector<int> &acc, std::vector<std::vector<Vertex>> &cs_dp) {
  if (index == size) {
    PrintCandidates(acc);
    Backtrack::count++;
    if (!checkAnswer(acc, data, query)) {
      Backtrack::count_error++;
    }
    if (count == MAX_SIZE)
      return true;
    return false;
  } else {
    int csNum = (cs_dp[index].empty()) ? cs.GetCandidateSize(index) : cs_dp[index].size();
    std::vector<std::vector<Vertex>> cs_dp_next;
    bool isEmpty = cs_dp[index].empty();
    for (int _i = 0; _i < csNum; ++_i) {
      int ithCandidate = isEmpty ? cs.GetCandidate(index, _i) : cs_dp[index][_i];
      if (std::find(acc.begin(), acc.end(), ithCandidate) == acc.end() &&
          CheckNeighborsWithDP(data, query, cs, index, _i, cs_dp, cs_dp_next)) {
        acc[index] = ithCandidate;
        bool isEnd = CheckWithDP(data, query, cs, index + 1, size, acc, cs_dp_next);
        acc[index] = -1;
        if (isEnd)
          return true;
      }
    }
    return false;
  }
}
