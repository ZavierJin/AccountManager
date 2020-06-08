/* Answer.h
** Brief introduction: definition of Answer
** Create by Hu Jiangpeng 2020/5/18
** Modify:
*/

#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include "Address.h"

namespace answer
{
	using Date = double;    // just for debug
	using std::shared_ptr;  // Much safer pointer
	using Record = std::string;
	using network::Address;
	using std::string;
	using std::vector;
	const string error = "Can't access this attribute.";

/*****************************************************************************
***************** Section 1: Enumeration variable definition *****************
*****************************************************************************/
	enum class Kind // Identify Answer categories
	{
		BalanceAnswer,
		RecordAnswer,
		LogInAnswer,
		RegisterAnswer,
		DealAnswer,
		VoteAnswer,
		AdditionAnswer,
		FeedBack
	};

	enum class Attribute // Identify attribute categories
	{
		Amount,

		TradeRecord,

		LogInState,

		RegisterState,

		DealState,

		ReceiverTerm,
		Attitude,
		ReceiverId,

		IsLeader,
		LeaderId
	};

/*****************************************************************************
************** Section 2: Definition and inheritance of Answer **************
*****************************************************************************/

	/*  BaseClass: Answer
	**  Note:
	**  	1. User cannot create Answer directly and can only use them indirectly through pointers or references
	**  	2. If you access a property that does not exist in the actual object,
	**     an exception is thrown(std::runtime_error)
	**	Example:
	** 		(Accessing properties through pointers)
	**  	FigureAnswer amount_answer(money);
	**  	Answer *ptr = &amount_answer;
	**  	double my_money;
	**  	if (ptr->hasAttribute(Attribute::Amount))
	**     		my_money = ptr->getAmount();
	**  	try
	**  	{
	**   	   ptr->getTradeRecord(); // ptr can't access TradeRecord
	** 		}
	**  	catch (const std::runtime_error &e)
	**  	{
	**      	cout << e.what();
	**  	}
	*/
	class Answer
	{
	public:
		virtual ~Answer() = default;

		Kind getKind() const { return _kind; };
		void setAddress(const Address &in) const { _sender = in; }
		Address getAddress() const { return _sender; }

		// Check whether the object has corresponding properties
		virtual bool hasAttribute(Attribute atr) const;

		// InfoAnswer's Attribute
		virtual double getAmount() const;
		virtual Record getTradeRecord () const;
		virtual int getReceiverTerm() const;
		virtual bool getAttitude() const;
		virtual int getReceiverId()const;
			
		// StateAnswer's Attribute
		virtual int getLogInState() const;
		virtual int getRegisterState() const;
		virtual int getDealState() const;

		// Feedback's Attribute
		virtual bool getIsLeader() const;
		virtual int getLeaderId() const;

	protected:
		Answer(Kind rkind);
	private:
		Kind _kind;
		mutable Address _sender;
	};

	class RaftAnswer : public Answer
	{
	public:
		bool hasAttribute(Attribute atr) const override;
		int getReceiverTerm() const override { return _receiver_term; }
		bool getAttitude() const override { return _attitude; }

		Answer *clone() const { return new RaftAnswer(*this); }
	protected:
		RaftAnswer(const int receiver_term, const bool attitude, Kind rkind);
	private:
		int _receiver_term;
		bool _attitude;
	};

/*****************************************************************************
************************* Section 3: Concrete Answer ************************
*****************************************************************************/
// Note: This section defines specific classes that can be created by the receiver


/************************ Classes created by server ************************/
	class BalanceAnswer : public Answer
	{
	public:
		BalanceAnswer(const double amount);
		bool hasAttribute(Attribute atr) const override;
		double getAmount() const override { return _amount; }

		Answer *clone() const { return new BalanceAnswer(*this); }
	private:
		double _amount;
	};

	class RecordAnswer : public Answer
	{
	public:
		RecordAnswer(const Record &trade_record);
		bool hasAttribute(Attribute atr) const override;
		Record getTradeRecord() const override { return _trade_record; }

		Answer *clone() const { return new RecordAnswer(*this); }
	private:
		Record _trade_record;
	};

	class LogInAnswer: public Answer
	{
	public:
		LogInAnswer(const int login_state);
		bool hasAttribute(Attribute atr) const override;
		int getLogInState() const override { return _login_state; }

		Answer *clone() const { return new LogInAnswer(*this); }
	private:
		int _login_state;
	};

	class RegisterAnswer : public Answer
	{
	public:
		RegisterAnswer(const int register_state);
		bool hasAttribute(Attribute atr) const override;
		int getRegisterState() const override { return _register_state; }

		Answer *clone() const { return new RegisterAnswer(*this); }
	private:
		int _register_state;
	};

	class DealAnswer : public Answer
	{
	public:
		DealAnswer(const int deal_state);
		bool hasAttribute(Attribute atr) const override;
		int getDealState() const override { return _deal_state; }

		Answer *clone() const { return new DealAnswer(*this); }
	private:
		int _deal_state;
	};

	class VoteAnswer : public RaftAnswer
	{
	public:
		VoteAnswer(const int receiver_term, const bool attitude);
		Answer *clone() const { return new VoteAnswer(*this); }
	};

	class AdditionAnswer : public RaftAnswer
	{
	public:
		AdditionAnswer(const int receiver_term, const bool attitude, const int receiver_id);
		bool hasAttribute(Attribute atr) const override;
		Answer *clone() const { return new AdditionAnswer(*this); }
		int getReceiverId() const override { return _receiver_id; }
	private:
		int _receiver_id;
	};

	class FeedBack : public Answer
	{
	public:
		FeedBack(const bool is_leader, const int leader_id);
		bool hasAttribute(Attribute atr) const override;
		bool getIsLeader() const override { return _is_leader; }
		int getLeaderId() const override { return _leader_id; }

		Answer *clone() const { return new FeedBack(*this); }
	private:
		bool _is_leader;
		int _leader_id;
	};

} //namespace answer

