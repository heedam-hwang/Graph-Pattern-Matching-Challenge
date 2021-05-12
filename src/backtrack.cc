/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  int num = query.GetNumVertices();
  std::cout << "t " << num << "\n";
  std::vector<std::vector<int>> answer_list;

  for (int i = 0; i < cs.GetCandidateSize(0); i++)
  {
    recursive_check(data, query, cs, 0, num, answer_list);
    for (std::vector<int> &j: answer_list)
    {
      std::cout << "a";
      for (int &k: j)
      {
        std::cout << " "<< k ;
      }
      std::cout << "\n";
    }
    answer_list.clear();
  }
  

  // implement your code here.
}

// Maybe we can use label frequencey to make search fast

void Backtrack::recursive_check(const Graph& data, const Graph &query, const CandidateSet &cs, int index, int size, std::vector<std::vector<int>> acc, bool tf)
{
  if (!tf)
    return;
  
  int startOffset = query.GetNeighborStartOffset(index);
  int endOffset = query.GetNeighborEndOffset(index);
  int num = cs.GetCandidateSize(index);
  std::vector<int> tempVec;
  for (int i = 0; i < num; ++i)
  {
    int candidateIndex = cs.GetCandidate(index, i);
    // for candidate's Neighbor
    // check it has same label with query's Neighbor
    int candidateStartOffset = data.GetNeighborStartOffset(candidateIndex);
    int candidateEndOffset = data.GetNeighborEndOffset(candidateIndex);
    for (int j = startOffset; j < endOffset; ++j)
    {
      bool temp = false;
      int labelOfQ = query.GetLabel(j);
      for (int k = candidateStartOffset; k < candidateEndOffset; ++k)
      {
        if (labelOfQ == data.GetLabel(k))
        {
          temp = true;
          break;
        }
      }
      if (!temp)
        return;

    }

  }
}