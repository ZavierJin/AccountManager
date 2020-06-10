/* Network.h
** Brief introduction: definition of Network
** Create by Zhang Zhecheng 2020/5/19
** Modify:
**      2020/5/25 (by Zhang Zhecheng)
**          1. Add searchServer()
**          2. Make brige between Network and NetworkImpl
*/
/************* NOT TESTED ************/
#pragma once
#include <memory>
#include "NetworkImpl.h"

namespace network
{

    class Network
    {
    public:
        Network() : _impl(make_network()) {}
        bool addressExist(const Address &adr) const { return _impl->addressExist(adr); } 
        bool addressEmpty(const Address &adr) const { return _impl->addressEmpty(adr); } 

        bool sendMessage(const Message &msg, const Address &adr) {return _impl->sendMessage(msg,adr);} // Just for Debug
        Message getMessage(const Address &adr) { return _impl->getMessage(adr); } // Just for Debug

        const Address &getMyAddress() const { return _impl->getMyAddress(); }; // Just for Debug
        int registerServer() { return _impl->registerServer(); }

        std::vector<Address> searchServer() const { return _impl->searchServer(); }

    private:
        shared_ptr<NetworkImpl> _impl;
    };
}