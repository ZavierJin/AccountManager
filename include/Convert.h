/* ConvertFunction.h
** Brief introduction: Class converting Request and Answer to Message ...
** Create by Hu Jiangpeng 2020/5/28
** Modify
*/
/************* NOT TESTED ************/
#pragma once
#include "Request.h"
#include "Answer.h"
#include "Message.h"
#include <memory>
#include <sstream>
#include <vector>

namespace convert
{
    using answer::Answer;
    using network::Message;
    using request::Request;
    using std::shared_ptr;

	class Convert
	{
	public:
		static Message request2Message(const Request &req);
		static Message answer2Message(const Answer &ans);
		static shared_ptr<Request> message2Request(const Message &msg);
		static shared_ptr<Answer> message2Answer(const Message &msg);
	};
	
} // namespace convert

