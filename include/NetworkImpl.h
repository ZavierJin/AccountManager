/* NetwordImpl.h
** Brief introduction: definition of NetworkImpl
** Create by Zhang Zhecheng 2020/5/19
** Modify
**      2020/5/25 (by Zhang Zhecheng)
**          1. Implementation the Network's interface
**      2020/5/27 (by Zhang Zhecheng)
**          1. Move TxtNetworkImpl into namespace txt
**          2. Tested 
*/
#pragma once
#include "Address.h"
#include "Message.h"
#include <vector>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <chrono>
#include <process.h>
#include <algorithm>
#include <mutex>
#include <map>
#include <queue>
#include <random>
#include <atomic>

namespace network
{
    using std::shared_ptr;
    using std::ifstream;
    using std::ofstream;
    using std::ostringstream;


    class NetworkImpl
    {
    public:
        NetworkImpl(const Address & adr) :_my_address(adr) {}
        virtual ~NetworkImpl() = default;

        virtual bool addressExist(const Address &adr) = 0;                   
        virtual bool addressEmpty(const Address &adr) = 0;                   

        virtual bool sendMessage(const Message &msg, const Address &adr) = 0;
        virtual Message getMessage(const Address &adr) = 0;                   

        virtual int registerServer() = 0;
        virtual std::vector<Address> searchServer() const = 0;

        const Address &getMyAddress() const { return _my_address; };
    private:
        Address _my_address;
    };

    shared_ptr<NetworkImpl> make_network();

    namespace txt
    {
        const Address network_root = Address("C:\\network");

        class TxtNetworkImpl :public NetworkImpl
        {
        public:
            // TESTED (2020/5/27 by Zhang Zhecheng)
            TxtNetworkImpl();
            // TESTED (2020/5/27 by Zhang Zhecheng)
            ~TxtNetworkImpl();

            // TESTED (2020/5/27 by Zhang Zhecheng)
            bool addressExist(const Address &adr) override;

            // TESTED (2020/5/27 by Zhang Zhecheng)
            bool addressEmpty(const Address &adr) override;

            // TESTED (2020/5/27 by Zhang Zhecheng)
            bool sendMessage(const Message &msg, const Address &adr) override;

            // TESTED (2020/5/27 by Zhang Zhecheng) 
            Message getMessage(const Address &adr) override;
            
            // TESTED (2020/5/27 by Zhang Zhecheng) 
            int registerServer() override;

            // TESTED (2020/5/27 by Zhang Zhecheng) 
            std::vector<Address> searchServer() const override;

        protected:
            static std::string generateComputerName();
            std::string generateFileName() const;
            static bool fileCanBeRead(const Address &adr);
            static Address generateServerName(const Address &adr);
            static bool isServer(const Address &adr);
        /*private:
            static std::mutex _using_iterator;*/
        };

        inline Address TxtNetworkImpl::generateServerName(const Address &adr)
        {
            return adr.string() + ".myserver";
        }

        inline bool TxtNetworkImpl::fileCanBeRead(const Address &adr)
        {
            auto filename = adr.string();
            return fs::exists(Address(filename + ".canread"));
        }

        inline bool TxtNetworkImpl::isServer(const Address &adr)
        {
            return fs::exists(generateServerName(adr));
        }
        
        // Get the sub address of the request under the host address
        inline Address getRequestAddress(const Address &adr)
        {
            return adr / "request";
        }

        // Get the sub address of the answer under the host address
        inline Address getAnswerAddress(const Address &adr)
        {
            return adr / "answer";
        }

        inline Address getServerAddress(int ServerId)
        {
            return Address(std::to_string(ServerId));
        }
    }

    using txt::getAnswerAddress;
    using txt::getRequestAddress;
    using txt::getServerAddress;
} // namespace network
