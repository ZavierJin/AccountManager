// SignIn.cpp
// Brief introduction: the implementation of SignIn's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"
namespace request
{
    SignIn::SignIn(const string &username, const string &password)
        : UserRequest(username, Kind::SignIn), _password(password){}
    bool SignIn::hasAttribute(Attribute atr) const
    {
        bool ret;
        switch (atr)
        {
        case Attribute::Password : ret = true;break;
        default : ret=UserRequest::hasAttribute(atr);break;
        }
        return ret;
    }
} // namespace request
