// Vote.cpp
// Brief introduction: the implementation of Vote's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"

namespace request
{
    Vote::Vote(int candidate_term, int candidate_id, int lastLogIndex, int lastLogTerm)
        : RaftRequest(candidate_term, candidate_id, lastLogIndex, lastLogTerm, Kind::Vote){}
} // namespace request
