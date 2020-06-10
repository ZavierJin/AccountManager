// BalanceAnswer.cpp
// Brief introduction: the implementation of BalanceAnswer's method
// Create by Hu Jiangpeng 2020/5/18
#include "Answer.h"
namespace answer
{
	BalanceAnswer::BalanceAnswer(const double amount)
		: Answer(Kind::BalanceAnswer), _amount(amount) {}
	bool BalanceAnswer::hasAttribute(Attribute atr) const
	{
		bool ret;
		switch (atr)
		{
		case Attribute::Amount: ret = true; break;
		default: ret = Answer::hasAttribute(atr); break;
		}
		return ret;
	}
} // namespace answer
