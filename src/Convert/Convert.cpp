/* ConvertFunction.cpp
** Brief introduction: definition of convert-class
** Create by Hu Jiangpeng 2020/6/1
** Modify
*/
// TODO:
//      1. Define functions following
/************* NOT TESTED ************/
#pragma once
#include "Convert.h"
namespace convert
{
	Message Convert::request2Message(const Request & req)
    {
        thread_local std::stringstream _buffer;
		Message msg;
		int length;
		_buffer << (req.getAddress()).string() << "\n" << int(req.getKind());
		switch (req.getKind())
		{
			case request::Kind::Register:
				if (req.hasAttribute(request::Attribute::User))
					_buffer << "\n" << req.getUser();
				if (req.hasAttribute(request::Attribute::Password))
					_buffer << "\n" << req.getPassword();
				break;

			case request::Kind::SignIn:
				if (req.hasAttribute(request::Attribute::User))
					_buffer << "\n" << req.getUser();
				if (req.hasAttribute(request::Attribute::Password))
					_buffer << "\n" << req.getPassword();
				break;

			case request::Kind::Deal:
				if (req.hasAttribute(request::Attribute::User))
					_buffer << "\n" << req.getUser();
				if (req.hasAttribute(request::Attribute::AccountCollector))
					_buffer << "\n" << req.getAccountCollector();
				if (req.hasAttribute(request::Attribute::Account))
					_buffer << "\n" << req.getAccount();
				if (req.hasAttribute(request::Attribute::TransactionDate))
					_buffer << "\n" << req.getTransactionDate();
				break;

			case request::Kind::BalanceCheck:
				if (req.hasAttribute(request::Attribute::User))
					_buffer << "\n" << req.getUser();
				break;

			case request::Kind::RecordCheck:
				if (req.hasAttribute(request::Attribute::User))
					_buffer << "\n" << req.getUser();
				if (req.hasAttribute(request::Attribute::StartDate))
					_buffer << "\n" << req.getStartDate();
				if (req.hasAttribute(request::Attribute::EndDate))
					_buffer << "\n" << req.getEndDate();
				break;

		//	case request::Kind::Transmit:
		//		length = 1;
		//		_buffer << "\n" << length;
		//		if (req.hasAttribute(request::Attribute::Forwarder))
		//			_buffer << " " << req.getForwarder();
		//		break;

			case request::Kind::Vote:
				if (req.hasAttribute(request::Attribute::SenderTerm))
					_buffer << "\n" << req.getSenderTerm();
				if (req.hasAttribute(request::Attribute::SenderId))
					_buffer << "\n" << req.getSenderId();
				if (req.hasAttribute(request::Attribute::SenderLastLogIndex))
					_buffer << "\n" << req.getSenderLastLogIndex();
				if (req.hasAttribute(request::Attribute::SendLastLogTerm))
					_buffer << "\n" << req.getSenderLastLogTerm();
				break;

			case request::Kind::Addition:
				if (req.hasAttribute(request::Attribute::SenderTerm))
					_buffer << "\n" << req.getSenderTerm();
				if (req.hasAttribute(request::Attribute::SenderId))
					_buffer << "\n" << req.getSenderId();
				if (req.hasAttribute(request::Attribute::SenderLastLogIndex))
					_buffer << "\n" << req.getSenderLastLogIndex();
				if (req.hasAttribute(request::Attribute::SendLastLogTerm))
					_buffer << "\n" << req.getSenderLastLogTerm();
				if (req.hasAttribute(request::Attribute::LeaderCommit))
					_buffer << "\n" << req.getLeaderCommit();  
				if (req.hasAttribute(request::Attribute::Entries))
				{
					std::vector<std::string> entries;
					entries = req.getEntries();
					_buffer << "\n" << entries.size();
					for (auto it = entries.begin(); it != entries.end(); it++)
						_buffer << "\n" << *it;
				}
				break;
		}
		msg = _buffer.str();
		_buffer.str("");
		_buffer.clear();
        return msg;
    }

    Message Convert::answer2Message(const Answer &ans)
    {
        thread_local std::stringstream _buffer;
		Message msg;
		_buffer << (ans.getAddress()).string() << "\n" << int(ans.getKind());
		switch (ans.getKind())
		{
		case answer::Kind::BalanceAnswer:
			if (ans.hasAttribute(answer::Attribute::Amount))
				_buffer << "\n" << ans.getAmount();
			break;

		case answer::Kind::RecordAnswer:
			if (ans.hasAttribute(answer::Attribute::TradeRecord))
				_buffer << "\n" << ans.getTradeRecord();
			break;

		case answer::Kind::LogInAnswer:
			if (ans.hasAttribute(answer::Attribute::LogInState))
				_buffer << "\n" << ans.getLogInState();
			break;

		case answer::Kind::RegisterAnswer:
			if (ans.hasAttribute(answer::Attribute::RegisterState))
				_buffer << "\n" << ans.getRegisterState();
			break;

		case answer::Kind::DealAnswer:
			if (ans.hasAttribute(answer::Attribute::DealState))
				_buffer << "\n" << ans.getDealState();
			break;

		case answer::Kind::VoteAnswer:
			if (ans.hasAttribute(answer::Attribute::ReceiverTerm))
				_buffer << "\n" << ans.getReceiverTerm();
			if (ans.hasAttribute(answer::Attribute::Attitude))
				_buffer << "\n" << ans.getAttitude();
			break;

		case answer::Kind::AdditionAnswer:
			if (ans.hasAttribute(answer::Attribute::ReceiverTerm))
				_buffer << "\n" << ans.getReceiverTerm();
			if (ans.hasAttribute(answer::Attribute::Attitude))
				_buffer << "\n" << ans.getAttitude();
			if (ans.hasAttribute(answer::Attribute::ReceiverId))
				_buffer << "\n" << ans.getReceiverId();
			break;

		case answer::Kind::FeedBack:
			if (ans.hasAttribute(answer::Attribute::IsLeader))
				_buffer << "\n" << ans.getIsLeader();
			if (ans.hasAttribute(answer::Attribute::LeaderId))
				_buffer << "\n" << ans.getLeaderId();
			break;
		}
		msg = _buffer.str();
		_buffer.str("");
		_buffer.clear();
        return msg;
    } 

    shared_ptr<Request> Convert::message2Request(const Message &msg) 
    {
        thread_local std::stringstream _buffer;
		_buffer << msg;

		std::string string_address;
		std::getline(_buffer, string_address);
		network::Address talker_address(string_address);

		request::Kind request_kind;
		int kind;
		_buffer >> kind;
		request_kind = request::Kind(kind);
		shared_ptr<Request> ptr;
		switch (request_kind)
		{
			case request::Kind::Register:
			{
				std::string user, password;
				std::getline(_buffer, user); //¶ÁÈ¡»»ĞĞ·û
				std::getline(_buffer, user);
				std::getline(_buffer, password);
				ptr = std::make_shared<request::Register>(user, password);
				ptr->setAddress(talker_address);
			}
				break;

			case request::Kind::SignIn:
			{
				std::string user, password;
				std::getline(_buffer, user); //¶ÁÈ¡»»ĞĞ·û
				std::getline(_buffer, user); 
				std::getline(_buffer, password);
				ptr = std::make_shared<request::SignIn>(user, password);
				ptr->setAddress(talker_address);
			}
				break;

			case request::Kind::Deal:
			{
				std::string user, account_collector;
				request::Date date;
				double volume;
				std::getline(_buffer, user); //¶ÁÈ¡»»ĞĞ·û
				std::getline(_buffer, user);
				std::getline(_buffer, account_collector);
				_buffer >> volume >> date;
				ptr = std::make_shared<request::Deal>(user, account_collector, volume, date);
				ptr->setAddress(talker_address);
			}
				break;

			case request::Kind::BalanceCheck:
			{
				std::string user;
				std::getline(_buffer, user); //¶ÁÈ¡»»ĞĞ·û
				std::getline(_buffer, user);
				ptr = std::make_shared<request::BalanceCheck>(user);
				ptr->setAddress(talker_address);
			}
				break;

			case request::Kind::RecordCheck:
			{
				std::string user;
				request::Date start_date, end_date;
				std::getline(_buffer, user); //¶ÁÈ¡»»ĞĞ·û
				std::getline(_buffer, user);
				_buffer >> start_date >> end_date;
				ptr = std::make_shared<request::RecordCheck>(user,start_date, end_date);
				ptr->setAddress(talker_address);
			}
				break;

		//	case request::Kind::Transmit:
		//		break;

			case request::Kind::Vote:
			{
				int sender_term, sender_id, sender_last_log_index, sender_last_log_trem;
				_buffer >> sender_term >> sender_id >> sender_last_log_index
					>> sender_last_log_trem;
				ptr = std::make_shared<request::Vote>(sender_term, sender_id,
					sender_last_log_index, sender_last_log_trem);
				ptr->setAddress(talker_address);
			}
				break;

			case request::Kind::Addition:
			{
				int sender_term, sender_id, sender_last_log_index,
					sender_last_log_trem, leader_commit;
				int size;
				std::vector<std::string> entries;
				std::vector<std::string> box;

				_buffer >> sender_term >> sender_id >> sender_last_log_index
					>> sender_last_log_trem >> leader_commit;
				_buffer >> size;
				

				std::string each_one;
				std::getline(_buffer, each_one); // ¶Á»»ĞĞ·û

				for (int i = 0; i < size; ++i)
				{
					std::getline(_buffer, each_one);
					entries.push_back(each_one);
				}

				ptr = std::make_shared<request::Addition>(sender_term, sender_id, sender_last_log_index,
					sender_last_log_trem, leader_commit, entries.begin(), entries.end());
				ptr->setAddress(talker_address);
				return ptr;
			}
				break;
		}
		_buffer.str("");
		_buffer.clear();
        return ptr;
    }// Just for Debug

    shared_ptr<Answer> Convert::message2Answer(const Message &msg)
    {
        thread_local std::stringstream _buffer;
		_buffer << msg;
		std::string string_address;
		std::getline(_buffer, string_address);
		network::Address answer_address(string_address);

		answer::Kind answer_kind;
		int kind;
		_buffer >> kind;
		answer_kind = (answer::Kind)kind;
		shared_ptr<Answer> ptr;
		switch (answer_kind)
		{
			case answer::Kind::BalanceAnswer:
			{
				double amount;
				_buffer >> amount;
				ptr = std::make_shared<answer::BalanceAnswer>(amount);
				ptr->setAddress(answer_address);
			}
				break;

			case answer::Kind::RecordAnswer:
			{
				std::string record;
				std::getline(_buffer, record);
				std::getline(_buffer, record);
				ptr = std::make_shared<answer::RecordAnswer>(record);
				ptr->setAddress(answer_address);
			}
				break;

			case answer::Kind::LogInAnswer:
			{
				int log_in_state;
				_buffer >> log_in_state;
				ptr = std::make_shared<answer::LogInAnswer>(log_in_state);
				ptr->setAddress(answer_address);
			}
				break;

			case answer::Kind::RegisterAnswer:
			{
				int register_state;
				_buffer >> register_state;
				ptr = std::make_shared<answer::RegisterAnswer>(register_state);
				ptr->setAddress(answer_address);
			}
				break;

			case answer::Kind::DealAnswer:
			{
				int deal_state;
				_buffer >> deal_state;
				ptr = std::make_shared<answer::DealAnswer>(deal_state);
				ptr->setAddress(answer_address);
			}
				break;

			case answer::Kind::VoteAnswer:
			{
				int receiver_term;
				bool attitude;
				_buffer >> receiver_term >> attitude;
				ptr = std::make_shared<answer::VoteAnswer>(receiver_term, attitude);
				ptr->setAddress(answer_address);
			}
				break;

			case answer::Kind::AdditionAnswer:
			{
				int receiver_term, receiver_id;
				bool attitude;
				_buffer >> receiver_term >> attitude >> receiver_id;
				ptr = std::make_shared<answer::AdditionAnswer>(receiver_term, attitude, receiver_id);
				ptr->setAddress(answer_address);
			}
				break;

			case answer::Kind::FeedBack:
			{
				bool is_leader;
				int leader_id;
				_buffer >> is_leader >> leader_id;
				ptr = std::make_shared<answer::FeedBack>(is_leader, leader_id);
				ptr->setAddress(answer_address);
			}
				break;
		}
		_buffer.str("");
		_buffer.clear();
        return ptr;
    }
	// Just for Debug
} // namespace convert
