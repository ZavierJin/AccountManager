// FeedBack.cpp
// Brief introduction: the implementation of FeedBack's method
// Create by Hu Jiangpeng 2020/5/18
#include "Answer.h"
namespace answer
{
	FeedBack::FeedBack(const bool is_leader, const int leader_id)
		: Answer(Kind::FeedBack), _is_leader(is_leader),_leader_id(leader_id){}
	bool FeedBack::hasAttribute(Attribute atr) const
	{
		bool ret;
		switch (atr)
		{
		case Attribute::IsLeader:
		case Attribute::LeaderId: 
			ret = true; 
			break;
		default: ret = Answer::hasAttribute(atr); break;
		}
		return ret;
	}
} // namespace answer
