// AdditionAnswer.cpp
// Brief introduction: the implementation of AdditionAnswer's method
// Create by Hu Jiangpeng 2020/6/1
#include "Answer.h"

namespace answer
{
	AdditionAnswer::AdditionAnswer(const int receiver_term, const bool attitude, const int receiver_id)
		: RaftAnswer(receiver_term, attitude, Kind::AdditionAnswer), _receiver_id(receiver_id) {}
	bool AdditionAnswer::hasAttribute(Attribute atr) const
	{
		bool ret;
		switch (atr)
		{
		case Attribute::ReceiverTerm:
		case Attribute::Attitude:
			ret = true;
			break;
		case Attribute::ReceiverId:
			ret = true;
			break;
		default: ret = Answer::hasAttribute(atr); break;
		}
		return ret;
	}
} // namespace request
