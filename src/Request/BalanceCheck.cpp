// BalanceCheck.cpp
// Brief introduction: the implementation of BalanceCheck's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"
namespace request
{
    BalanceCheck::BalanceCheck(const string &username)
        : UserRequest(username, Kind::BalanceCheck){}
} // namespace request
