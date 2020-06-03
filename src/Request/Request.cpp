// Request.cpp
// Brief introduction: the implementation of Request's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"

namespace request
{
    Request::Request(Kind rkind) : _kind(rkind){}

    bool Request::hasAttribute(Attribute atr) const
    {
        return false;
    }



    // UserRequest's Attribute
    string Request::getUser() const
    {
        throw std::runtime_error(error);
    }

    // Register and SignIn's Attribute
    string Request::getPassword() const
    {
        throw std::runtime_error(error);
    }

    // Deal's Attribute
    string Request::getAccountCollector() const
    {
        throw std::runtime_error(error);
    }
    double Request::getAccount() const
    {
        throw std::runtime_error(error);
    }
    Date Request::getTransactionDate() const
    {
        throw std::runtime_error(error);
    }

    // RecordCheck's Attribute
    Date Request::getStartDate() const
    {
        throw std::runtime_error(error);
    }
    Date Request::getEndDate() const
    {
        throw std::runtime_error(error);
    }



    // Transmit's Attribute
    shared_ptr<Request> Request::getRequest() const
    {
        throw std::runtime_error(error);
    }
    string Request::getForwarder() const
    {
        throw std::runtime_error(error);
    }




    // RaftRequest's Attribute
    int Request::getSenderTerm() const
    {
        throw std::runtime_error(error);
    }
    int Request::getSenderId() const
    {
        throw std::runtime_error(error);
    }
    int Request::getSenderLastLogIndex() const
    {
        throw std::runtime_error(error);
    }
    int Request::getSenderLastLogTerm() const
    {
        throw std::runtime_error(error);
    }

    // Addition's Attribute
    vector<std::string>  Request::getEntries() const
    {
        throw std::runtime_error(error);
    }
    int Request::getLeaderCommit() const
    {
        throw std::runtime_error(error);
    }
}