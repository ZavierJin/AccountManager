// DealAnswer.cpp
// Brief introduction: the implementation of DealAnswer's method
// Create by Hu Jiangpeng 2020/5/18
#include "Answer.h"
namespace answer
{
	DealAnswer::DealAnswer(const int deal_state)
		: Answer(Kind::DealAnswer), _deal_state(deal_state) {}
	bool DealAnswer::hasAttribute(Attribute atr) const
	{
		bool ret;
		switch (atr)
		{
		case Attribute::DealState: ret = true; break;
		default: ret = Answer::hasAttribute(atr); break;
		}
		return ret;
	}
} // namespace answer
