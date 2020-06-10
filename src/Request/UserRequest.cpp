// UserRequest.cpp
// Brief introduction: the implementation of UserRequest's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"
namespace request
{
    UserRequest::UserRequest(const string &username, Kind rkind)
        : Request(rkind), _user(username){}
    bool UserRequest::hasAttribute(Attribute atr) const
    {
        bool ret;
        switch (atr)
        {
        case Attribute::User:
            ret = true;
            break;
        default:
            ret = Request::hasAttribute(atr);
            break;
        }
        return ret;
    }
} // namespace request
