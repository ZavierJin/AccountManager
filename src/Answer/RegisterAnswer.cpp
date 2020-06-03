// RegisterAnswer.cpp
// Brief introduction: the implementation of RegisterAnswer's method
// Create by Hu Jiangpeng 2020/5/18
#include "Answer.h"
namespace answer
{
	RegisterAnswer::RegisterAnswer(const int register_state)
		: Answer(Kind::RegisterAnswer), _register_state(register_state) {}
	bool RegisterAnswer::hasAttribute(Attribute atr) const
	{
		bool ret;
		switch (atr)
		{
		case Attribute::RegisterState: ret = true; break;
		default: ret = Answer::hasAttribute(atr); break;
		}
		return ret;
	}
} // namespace answer
