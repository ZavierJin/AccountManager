/*  Address.cpp
**  Create by Zhang Zhecheng 2020/5/19
**  Modify:
**      2020/5/24 (by Zhang Zhecheng)
**          1. Add getServerAddress(int)
*/
// TODO:
//      1. Define functions following
#include "Address.h"
namespace network
{
    // Get the sub address of the request under the host address
    Address getRequestAddress(const Address &adr)
    {
        return adr / "request";
    } 
    // Get the sub address of the answer under the host address
    Address getAnswerAddress(const Address &adr)
    {
        return adr / "answer";
    } 
    Address getServerAddress(int ServerId)
    {
        return Address(std::to_string(ServerId));
    }
}