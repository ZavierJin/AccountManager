// Regisster.cpp
// Brief introduction: the implementation of Register's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"
namespace request
{
    Register::Register(const string &username, const string &password)
        : UserRequest(username, Kind::Register), _password(password){}
    bool Register::hasAttribute(Attribute atr) const
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
