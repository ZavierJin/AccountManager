// Deal.cpp
// Brief introduction: the implementation of Deal's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"
namespace request
{
    Deal::Deal(const string &username, const string &dest, double volume, const Date &date)
        : UserRequest(username, Kind::Deal), _account_collector(dest), _volume(volume),_date(date){};
    bool Deal::hasAttribute(Attribute atr) const
    {
        bool ret;
        switch (atr)
        {
        case Attribute::AccountCollector:
        case Attribute::Account:
        case Attribute::TransactionDate:
            ret = true;
            break;
        default : ret=UserRequest::hasAttribute(atr);break;
        }
        return ret;
    }
} // namespace request
