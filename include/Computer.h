/* Computer.h
** Brief introduction: definition of multithreaded Computer
** Create by Zhang Zhecheng 2020/5/19
** Modify:
**      2020/5/20 (by Zhang Zhecheng)
**          1. Change queue to deque
**      2020/5/24 (by Zhang Zhecheng)
**          1. Add getRequest(request::Kind) 
**          2. Add getAnswer(answer::Kind)
**          3. Add hasRequest(request::Kind)
**          4. Add hasAnswer(answer::Kind)
**          5. Add registerServer()
**          6. Add sendRequest(Request &, int)
**      2020/5/25 (by Zhang Zhecheng)
**          1. Add getNextRequest()
**      2020/5/29 (by Zhang Zhecheng)
**          1. getReqeust -> getRequest
*/
/************* NOT TESTED ************/
#pragma once
//#define SINGLE_DEBUG

#include <queue>
#include <algorithm>
#include <random>

#include <thread>
#include <mutex>

#include "Network.h"


#include "Convert.h"

namespace computer
{
    using answer::Answer;
    using network::Address;
    using network::Network;
    using request::Request;
    using std::mutex;
    using std::deque;
    using std::shared_ptr;
    using convert::Convert;
    /*  Class: Computer
    **  Note:
    **      1. Each program will have only one Computer's instance
    **      2. Multithreading support
    **  Example:
    **      auto & com = computer::Computer::instance();
    **
    **      int server_id=com.registerServer();
    **      auto servers=com.getOnlineServer();
    **      while (servers.size()!=required_number)
    **          servers=com.getOnlineServer();
    **
    **      Register req("example","passward");
    **      com.sendRequest(req);
    **      auto ans=com.getAnswer();
    **      while (ans==nullptr)
    **          ans=com.getAnswer();    
    **
    **      auto req2=com.getRequest(request::Kind::Addition);
    **      while (!req2)
    **          req2=com.getRequest(request::Kind::Addition);
    **
    **      AdditonAnswer ans2(...) 
    **      com.sendAnswer(ans2, req2->getAddress());
    */
    class Computer
    {
    public:
        Computer(const Computer &) = delete;
        Computer &operator=(const Computer &) = delete;

        // Get computer's instance
        static Computer &instance();

        // Check for unprocessed answers
        bool hasAnswer() const;
        bool hasAnswer(answer::Kind);

        // Check for unprocessed requests
        bool hasRequest() const;
        bool hasRequest(request::Kind);
        // a runtime_error is thrown if there are no unprocessed requests
        request::Kind getNextRequestKind();

        // Send request used by client
        bool sendRequest(const Request &);
        // Send request used by server
        bool sendRequest(const Request &, const Address &);
        bool sendRequest(const Request &, int);
        // Send Answer
        bool sendAnswer(const Answer &, const Address &);

        // Get unprocessed requests
        // A null pointer is returned if there are no unprocessed requests
        shared_ptr<Request> getRequest();
        shared_ptr<Request> getRequest(request::Kind);
        // Get unprocessed answers
        // A null pointer is returned if there are no unprocessed requests
        shared_ptr<Answer> getAnswer();
        shared_ptr<Answer> getAnswer(answer::Kind);

        // Be a server
        // return server's ID
        int registerServer();
        // Get all online server
        // Include yourself
        std::vector<int> getOnlineServer() const;
        Address getMyAddress() const { return _network.getMyAddress(); }

    protected:
        Computer();
        virtual ~Computer();

        Address getRecipient() const;
        void loadAllRequests();
        void loadAllAnswers();
    private:
        deque<shared_ptr<Request>> _request_queue;
        mutable mutex _request_queue_mutex;
        deque<shared_ptr<Answer>> _answer_queue;
        mutable mutex _answer_queue_mutex;
        Network _network;
        int _server_id;
        bool _is_server;

        std::atomic<bool> _answer_thread_started = false;
        std::atomic<bool> _reqest_thread_started = false;
        std::atomic<bool> _thread_exit = false;
        // The background thread
        std::thread _load_answer_thread;
        std::thread _load_request_thread;

        // Singlon
        static Computer *_p_instance;
        static std::mutex _create_mutex;
        friend class Cleanup;
        class Cleanup
        {
            public:
                ~Cleanup();
        };
    };
} // namespace computer
