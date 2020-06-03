// Request.cpp
// Brief introduction: the implementation of Answer's method
// Create by Hu Jiangpeng 2020/6/1
#include "Answer.h"

namespace answer
{
	Answer::Answer(Kind rkind) : _kind(rkind) {}

	inline bool Answer::hasAttribute(Attribute atr) const
	{
		return false;
	}

	// BalanceAnswer's Attribute
	double Answer::getAmount() const
	{
		throw std::runtime_error(error);
	}

	// RececordAnswer's Attribute
	Record Answer::getTradeRecord() const
	{
		throw std::runtime_error(error);
	}

	// LogInAnswer's Attribute
	int Answer::getLogInState() const
	{
		throw std::runtime_error(error);
	}

	// RegisterAnswer's Attribute
	int Answer::getRegisterState() const
	{
		throw std::runtime_error(error);
	}

	// DealAnswer's Attribute
	int Answer::getDealState() const
	{
		throw std::runtime_error(error);
	}

	// Feedback's Attribute
	bool Answer::getIsLeader() const
	{
		throw std::runtime_error(error);
	}
	
	int Answer::getLeaderId() const
	{
		throw std::runtime_error(error);
	}

	// RaftAnswer's Attribute
	bool Answer::getAttitude() const
	{
		throw std::runtime_error(error);
	}
	int Answer::getReceiverId() const
	{
		throw std::runtime_error(error);
	}
	int Answer::getReceiverTerm() const
	{
		throw std::runtime_error(error);
	}

}