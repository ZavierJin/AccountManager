// RecordCheck.cpp
// Brief introduction: the implementation of RecordCheck's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"
namespace request
{
    RecordCheck::RecordCheck(const string &username, const Date &start, const Date &end)
        : UserRequest(username, Kind::RecordCheck), _start(start), _end(end){}
    bool RecordCheck::hasAttribute(Attribute atr) const
    {
        bool ret;
        switch (atr)
        {
        case Attribute::StartDate:
        case Attribute::EndDate:
            ret = true;
            break;
        default : ret=UserRequest::hasAttribute(atr);break;
        }
        return ret;
    }
} // namespace request
