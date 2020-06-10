// RececordAnswer.cpp
// Brief introduction: the implementation of RececordAnswer's method
// Create by Hu Jiangpeng 2020/5/18
#include "Answer.h"
namespace answer
{
	RecordAnswer::RecordAnswer(const Record &trade_record)
		: Answer(Kind::RecordAnswer), _trade_record(trade_record) {}
	bool RecordAnswer::hasAttribute(Attribute atr) const
	{
		bool ret;
		switch (atr)
		{
		case Attribute::TradeRecord: ret = true; break;
		default: ret = Answer::hasAttribute(atr); break;
		}
		return ret;
	}
} // namespace answer
