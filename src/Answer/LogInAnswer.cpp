// LogInAnswer.cpp
// Brief introduction: the implementation of LogInAnswer's method
// Create by Hu Jiangpeng 2020/5/18
#include "Answer.h"
namespace answer
{
	LogInAnswer::LogInAnswer(const int login_state)
		: Answer(Kind::LogInAnswer), _login_state(login_state) {}
	bool LogInAnswer::hasAttribute(Attribute atr) const
	{
		bool ret;
		switch (atr)
		{
		case Attribute::LogInState: ret = true; break;
		default: ret = Answer::hasAttribute(atr); break;
		}
		return ret;
	}
} // namespace answer
