// Addition.cpp
// Brief introduction: the implementation of Addition's method
// Create by Zhang Zhecheng 2020/5/18
#include "Request.h"
namespace request
{
    bool Addition::hasAttribute(Attribute atr) const
    {
        bool ret;
        switch (atr)
        {
        case Attribute::Entries:
        case Attribute::LeaderCommit:
            ret = true;
            break;
        default:
            ret = RaftRequest::hasAttribute(atr);
            break;
        }
        return ret;
    }
} // namespace request
