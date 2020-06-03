// RaftRequest.cpp
// Brief introduction: the implementation of RaftRequest's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"
namespace request
{
    RaftRequest::RaftRequest(int sender_term, int sender_id, int sender_last_log_index,int sender_last_log_term, Kind rkind)
        : Request(rkind), _sender_term(sender_term), _sender_id(sender_id), 
        _sender_last_log_index(sender_last_log_index),_sender_last_log_trem(sender_last_log_term){}

    bool RaftRequest::hasAttribute(Attribute atr) const
    {
        bool ret;
        switch (atr)
        {
        case Attribute::SenderId:
        case Attribute::SenderTerm:
        case Attribute::SenderLastLogIndex:
        case Attribute::SendLastLogTerm:
            ret = true;
            break;
        default:
            ret = Request::hasAttribute(atr);
            break;
        }
        return ret;
    }
} // namespace request
