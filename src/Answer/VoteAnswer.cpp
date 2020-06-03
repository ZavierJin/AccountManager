// VoteAnswer.cpp
// Brief introduction: the implementation of VoteAnswer's method
// Create by Hu Jiangpeng 2020/5/18
#include "Answer.h"

namespace answer
{
	VoteAnswer::VoteAnswer(const int receiver_term, const bool attitude)
		: RaftAnswer(receiver_term, attitude, Kind::VoteAnswer) {}
} // namespace request
