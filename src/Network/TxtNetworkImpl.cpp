/*  TxtNetworkImpl.cpp
**  Create by Zhang Zhecheng 2020/5/24
**  Modify:
**      1. Fixed an important BUG
*/
#include "NetworkImpl.h"

namespace network
{
    shared_ptr<NetworkImpl> make_network()
    {
        return std::make_shared<txt::TxtNetworkImpl>();
    }
    namespace txt
    {
        //std::mutex TxtNetworkImpl::_using_iterator;

        TxtNetworkImpl::TxtNetworkImpl() :NetworkImpl(generateComputerName())
        {
            auto dir = network_root / getMyAddress();
            fs::create_directories(dir);
            fs::create_directory(dir / "request");
            fs::create_directory(dir / "answer");
        }

        TxtNetworkImpl::~TxtNetworkImpl()
        {
            if (fs::exists(generateServerName(network_root / getMyAddress())))
                fs::remove(generateServerName(network_root / getMyAddress()));
            if (fs::exists(network_root / getMyAddress()))
                fs::remove_all(network_root / getMyAddress());
        }

        bool TxtNetworkImpl::addressExist(const Address &adr)
        {
            return fs::exists(network_root / adr);
        }

        bool TxtNetworkImpl::addressEmpty(const Address &adr)
        {
            //std::unique_lock<std::mutex> lock(_using_iterator);
            if (fs::is_empty(network_root / adr))
                return true;
            fs::directory_iterator dir_iter(network_root / adr);
            fs::directory_iterator end;
            // Find readable file
            auto target = std::find_if(dir_iter, end, [](decltype(*dir_iter) file)
            {
                auto path = file.path();
                return fs::is_regular_file(path) && fileCanBeRead(path);
            }
            );

            return target == end;
        }

        bool TxtNetworkImpl::sendMessage(const Message &msg, const Address &adr)
        {
            if (!addressExist(adr))
                return false;

            auto filename = generateFileName();
            auto filepath = network_root / adr / (filename+".myread");

            // True file to write
            ofstream fout(filepath.string());
            fout << msg << std::endl;
            fout.close();

            // When completing writing, remove the first file
            ofstream allow_file(filepath.string()+".canread");
            return true;
        }

        Message TxtNetworkImpl::getMessage(const Address &adr)
        {
            // Reduce duplicate lookups
            thread_local std::map<Address, std::deque<Address>> message_addresses;
            std::deque<Address> &target_deque = message_addresses[adr];

            if (target_deque.empty() && !fs::is_empty(network_root/adr))
            {
                //std::unique_lock<std::mutex> lock(_using_iterator);

                fs::directory_iterator dir_iter(network_root / adr), end;
                // Find readable file
                std::copy_if(dir_iter,end,std::back_inserter(target_deque), [](decltype(*dir_iter) file)
                    {
                        auto path = file.path();
                        return fs::is_regular_file(path) && fileCanBeRead(path);
                    }
                );
            }
            Message ret;
            // No message
            if (!target_deque.empty())
            {
                // Read message from file
                Address target = target_deque.front();
                target_deque.pop_front();

                ifstream fin(target.string());
                std::string temp;
                while (getline(fin, temp))
                    ret += temp + "\n";
                fin.close();

                if (ret == "")
                    throw std::exception("get");
                // Delete read file
                
                //std::unique_lock<std::mutex> lock(_using_iterator);
                fs::remove(target);
				boost::system::error_code ec;
                fs::remove(target.string() + ".canread", ec);
				while( ec.failed() )
					fs::remove(target.string() + ".canread", ec);
            }

            return ret;
        }

        int TxtNetworkImpl::registerServer()
        {
            ofstream fout(generateServerName(network_root / getMyAddress()).string());
            static std::istringstream in;
            int i;
            // string to int
            in.str(getMyAddress().string());
            in >> i;
            in.str();

            return i;
        }
        std::vector<Address> TxtNetworkImpl::searchServer() const
        {
            std::vector<Address> servers;

            // find .s file's address
            //std::unique_lock<std::mutex> lock(_using_iterator);
            fs::directory_iterator dir_iter(network_root), end;
            std::copy_if(dir_iter, end,
                std::back_inserter(servers),
                [](decltype(*dir_iter) file) {return isServer(file.path()); });
            //lock.unlock();

            // Get Server address
            std::for_each(servers.begin(), servers.end(), [](Address &adr) {adr = adr.stem(); });
            return std::move(servers);
        }
        std::string TxtNetworkImpl::generateComputerName()
        {
            int id = _getpid();
            return std::to_string(id);
        }
        std::string TxtNetworkImpl::generateFileName() const
        {
            ostringstream ret;
            static std::atomic<unsigned> order = 0;
            ret << time << "_";
            ret << getMyAddress().string() << "_"<< order++;
            return ret.str();
        }
    }
} // namespace network
