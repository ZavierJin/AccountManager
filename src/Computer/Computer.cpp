// Computer.cpp
// Brief introduction: the implementation of Computer's method
// Create by Zhang Zhecheng 2020/5/19
#include "Computer.h"

namespace computer
{
    using std::unique_lock;

    Computer *Computer::_p_instance = nullptr;
    mutex Computer::_create_mutex;
    
    // multithread
    Computer &Computer::instance()
    {
        // make the following code thread safe
        // Can't call method in Computer which has lock
        static Cleanup cleanup;             // clean up dynamic memory
        std::lock_guard<std::mutex> guard(Computer::_create_mutex);
        if (_p_instance==nullptr)
            _p_instance = new Computer;
        return *_p_instance;
    }

    Computer::Cleanup::~Cleanup()
    {
        delete Computer::_p_instance;
        Computer::_p_instance = nullptr;
    }

    Computer::Computer()
    {
        _load_answer_thread = std::thread(&Computer::loadAllAnswers, this);
        _load_request_thread = std::thread(&Computer::loadAllRequests, this);
        
        // Wait until background thread starts its processing loop.
        while (true)
            if (_answer_thread_started&&_reqest_thread_started)
                break;
    }
    Computer::~Computer()
    {
        _thread_exit = false;
        // Wait until thread is shut down
        _load_answer_thread.join();
        _load_request_thread.join();
    }

    bool Computer::hasAnswer() const
    {
        unique_lock<mutex> lock(_answer_queue_mutex);          // make the following code thread safe
        // Can't call method in Computer which has lock
        return  !_answer_queue.empty();
                                                 // no unprocessed answer in file
    }

    bool Computer::hasAnswer(answer::Kind kind)
    {
        unique_lock<mutex> lock(_answer_queue_mutex);          // make the following code thread safe

        if (_answer_queue.empty())
            return false;
        // Check if there is a corresponding type of answer
        auto target = std::find_if(_answer_queue.begin(),
                                   _answer_queue.end(),
                                   [=](shared_ptr<Answer> ans) {
                                       return ans->getKind() == kind;
                                   });
        return !(target == _answer_queue.end());
    }

    bool Computer::hasRequest() const
    {
        unique_lock<mutex> lcok(_request_queue_mutex);  // make the following code thread safe
        // Can't call method in Computer which has lock

        return  !_request_queue.empty();
    }
    bool Computer::hasRequest(request::Kind kind)
    {
        unique_lock<mutex> lock(_request_queue_mutex);      // make the following code thread safe
        
        if (_request_queue.empty())
            return false;
        // Check if there is a corresponding type of request
        auto target = std::find_if(_request_queue.begin(),
                                   _request_queue.end(),
                                   [=](shared_ptr<Request> ans) {
                                       return ans->getKind() == kind;
                                   });
        return !(target == _request_queue.end());
    }
    
    request::Kind Computer::getNextRequestKind()
    {
        unique_lock<mutex> lock(_request_queue_mutex);  // make the following code thread safe
 
        if (_request_queue.empty())
            throw std::runtime_error("No request");
        else
            return _request_queue.front()->getKind();
    }

    bool Computer::sendRequest(const Request &req, const Address &adr)
    {
        // Fill in the sender
        req.setAddress(_network.getMyAddress());

        unique_lock<mutex> lock(_request_queue_mutex);      // make the following code thread safe
        auto msg = Convert::request2Message(req);
        return _network.sendMessage(msg,network::getRequestAddress(adr));
    }

    bool Computer::sendRequest(const Request &req)
    {
        if (!sendRequest(req, getRecipient()))
            return false;

#ifndef SINGLE_DEBUG
        // Check feedback
        while (true)
        {
            if (hasAnswer(answer::Kind::FeedBack))
            {
                auto ans = getAnswer(answer::Kind::FeedBack);
                if (ans->getIsLeader())
                    return true;
                else
                    return sendRequest(req, ans->getLeaderId());
            }
        }
#else
        return true;
#endif
    }

    bool Computer::sendRequest(const Request &req, int ServerId)
    {
        return sendRequest(req,network::getServerAddress(ServerId));
    }


    bool Computer::sendAnswer(const Answer &ans, const Address &adr)
    {
        ans.setAddress(_network.getMyAddress());

        unique_lock<mutex> lock(_answer_queue_mutex);  // make the following code thread safe
        auto msg = Convert::answer2Message(ans);
        return _network.sendMessage(msg,network::getAnswerAddress(adr));
    }

    shared_ptr<Request> Computer::getRequest()
    {
        shared_ptr<Request> ret = nullptr;

        unique_lock<mutex> guard(_request_queue_mutex);      // make the following code thread safe

        if (!_request_queue.empty())
        {
            ret = _request_queue.front();
            _request_queue.pop_front();
        }

        return ret;
    }

    shared_ptr<Request> Computer::getRequest(request::Kind kind)
    {
        shared_ptr<Request> ret = nullptr;

        unique_lock<mutex> lock(_request_queue_mutex);  // make the following code thread safe

        // Find if there are requests of the specified type in the queue
        auto target = std::find_if(_request_queue.begin(),
                                   _request_queue.end(),
                                   [=](shared_ptr<Request> req) {
                                       return req->getKind() == kind;
                                   });
        // if exists, get first
        if (target!=_request_queue.end())
        {
            ret = *target;
            _request_queue.erase(target);   // delete it in queue
        }

        return ret;
    }

    shared_ptr<Answer> Computer::getAnswer()
    {
        shared_ptr<Answer> ret = nullptr;

        unique_lock<mutex> lock(_answer_queue_mutex);  // make the following code thread safe

        if (!_answer_queue.empty())
        {
            ret = _answer_queue.front();
            _answer_queue.pop_front();
        }

        return ret;
    }

    shared_ptr<Answer> Computer::getAnswer(answer::Kind kind)
    {
        shared_ptr<Answer> ret = nullptr;

        unique_lock<mutex> lock(_answer_queue_mutex);  // make the following code thread safe

        // Find if there are answer of the specified type in the queue
        auto target = std::find_if(_answer_queue.begin(),
                                   _answer_queue.end(),
                                   [=](shared_ptr<Answer> ans) {
                                       return ans->getKind() == kind;
                                   });
        // if exists, get first
        if (target!=_answer_queue.end())
        {
            ret = *target;
            _answer_queue.erase(target);
        }

        return ret;
    }

    int Computer::registerServer()
    {
        auto ret = _network.registerServer();

        if (ret != -1)
        {
            _is_server = true;
            _server_id = ret;
        }

        return ret;
    }

    Address Computer::getRecipient() const
    {
        auto servers = _network.searchServer();

        // Check if there is an online server
        if (servers.empty())
#ifndef SINGLE_DEBUG
            throw std::runtime_error("No server");
#else
            return Address("No Server");
#endif

        static std::default_random_engine e;
        std::uniform_int_distribution<unsigned> u(0, servers.size() - 1);
        return servers[u(e)];
    }

    std::vector<int> Computer::getOnlineServer() const
    {
        auto servers = _network.searchServer();
        std::vector<int> server_ids;

        if (servers.empty())
            return server_ids;
        std::for_each(servers.begin(), servers.end(), [&](const Address &adr) 
        {
            static std::istringstream is;
            is.str(adr.string());
            int id;
            is >> id;
            server_ids.push_back(id);
            is.clear();
            is.str("");
        });
        return std::move(server_ids);
    }

    void Computer::loadAllRequests()
    {
        Address request_address = network::getRequestAddress(_network.getMyAddress());
        unique_lock<mutex> lock(_request_queue_mutex);
        _reqest_thread_started = true;
        lock.unlock();
        while (true)
        {
            while (true)
                if (!_network.addressEmpty(request_address))
                {
                    auto msg = _network.getMessage(request_address);
                    lock.lock();
                    _request_queue.push_back(Convert::message2Request(msg));
                    lock.unlock();
                }
                else
                    break;
            if (_thread_exit)
                break;
        }
    }

    void Computer::loadAllAnswers()
    {
        Address answer_address = network::getAnswerAddress(_network.getMyAddress());

        unique_lock<mutex> lock(_answer_queue_mutex);
        _answer_thread_started = true;
        lock.unlock();

        while (true)
        {
            while (true)
                if (!_network.addressEmpty(answer_address))
                {
                    auto msg = _network.getMessage(answer_address);
                    lock.lock();
                    _answer_queue.push_back(Convert::message2Answer(msg));
                    lock.unlock();
                }
                else
                    break;
            if (_thread_exit)
                break;
        }
    }
} // namespace computer
