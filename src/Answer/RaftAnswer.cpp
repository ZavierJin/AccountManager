// RaftAnswer.cpp
// Brief introduction: the implementation of RaftAnswer's method
// Create by Hu Jiangpeng 2020/6/1
#include "Answer.h"
namespace answer
{
	RaftAnswer::RaftAnswer(const int receiver_term, const bool attitude, Kind rkind)
		: Answer(rkind), _receiver_term(receiver_term), _attitude(attitude) {}
	bool RaftAnswer::hasAttribute(Attribute atr) const
	{
		bool ret;
		switch (atr)
		{
		case Attribute::ReceiverTerm:
		case Attribute::Attitude:
			ret = true;
			break;
		default: ret = Answer::hasAttribute(atr); break;
		}
		return ret;
	}
} // namespace answer
