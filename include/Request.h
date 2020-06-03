/* Request.h
** Brief introduction: definition of request related classes
** Create by Zhang Zhecheng 2020/5/18
** Modify:
**      2020/5/18 (by Zhang Zhecheng): 
**          1. Attribute::Date -> Attribute::TransactionDate
**          2. getDate -> getTransactionDate 
**          3. Attribute::Volume -> Attribute::Account
**          4. getVolume -> getAccount
**          5. constructor of Transmit and Addition
**      2020/5/29 (by Zhang Zhecheng)
**          1. Comment out Transmit
**      2020/6/1 (by Zhang Zhecheng)
**          1. Change function getEntries's return type and Addtion's constructor
*/

#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include "Address.h"
#include <algorithm>

namespace request
{
    using Date = double;    // just for debug
    using network::Address;
    using std::shared_ptr;
    using std::string;
    using std::vector;
    const string error = "Can't access this attribute.";

/*****************************************************************************
***************** Section 1: Enumeration variable definition *****************
*****************************************************************************/
    enum class Kind // Identify request categories
    {
        Register,
        SignIn,
        Deal,
        BalanceCheck,
        RecordCheck,

        Transmit,

        Vote,
        Addition
    };

    enum class Attribute // Identify attribute categories
    {
        User,
        Password,
        AccountCollector,
        Account,
        TransactionDate,
        StartDate,
        EndDate,
        Request,
        Forwarder,
        SenderTerm,
        SenderId,
        SenderLastLogIndex,
        SendLastLogTerm,
        Entries,
        LeaderCommit
    };

/*****************************************************************************
************** Section 2: Definition and inheritance of Request **************
*****************************************************************************/

/*BaseClass:Request 
**Note: 
**  1. User cannot create request directly and can only use them indirectly through pointers or references
**  2. If you access a property that does not exist in the actual object, 
**     an exception is thrown(std::runtime_error)
**Example:
** (Accessing properties through pointers)
**  Register concrete_request("example","password");
**  Request *r=&concrete_request;
**  string username;
**  if (r->hasAttribute(Attribute::User))
**      username=r->getUser();
**  try 
**  {
**      r->getTransactionDate(); // r can't access Date
**  }
**  catch (const std::runtime_error &e)
**  {
**      cout << e.what();
**  }
*/
    class Request
    {
    public:
        virtual ~Request() = default;

        Kind getKind() const { return _kind; };
        void setAddress(const Address &in) const { _sender = in; }
        // Get reply address
        Address getAddress() const { return _sender; }

        // Check whether the object has corresponding properties
        virtual bool hasAttribute(Attribute atr) const;

        // UserRequest's Attribute
        virtual string getUser() const;
        // Register and SignIn's Attribute
        virtual string getPassword() const;
        // Deal's Attribute
        virtual string getAccountCollector() const;
        virtual double getAccount() const;
        virtual Date getTransactionDate() const;
        // RecordCheck's Attribute
        virtual Date getStartDate() const;
        virtual Date getEndDate() const;

        // Transmit's Attribute
        virtual shared_ptr<Request> getRequest() const;
        virtual string getForwarder() const;

        // RaftRequest's Attribute
        virtual int getSenderTerm() const;
        virtual int getSenderId() const;
        virtual int getSenderLastLogIndex() const;
        virtual int getSenderLastLogTerm() const;

        // Addition's Attribute
        virtual vector<string> getEntries() const;
        virtual int getLeaderCommit() const;

        virtual Request *clone() const = 0;

    protected:
        Request(Kind rkind);
    private:
        Kind _kind;
        mutable Address _sender;
    };

    class UserRequest : public Request
    {
    public:
        bool hasAttribute(Attribute atr) const override;
        string getUser() const override { return _user; }
    protected:
        UserRequest(const string &username, Kind rkind);
    private:
        string _user;
    };

    class RaftRequest:public Request
    {
    public:
        bool hasAttribute(Attribute atr) const override;
        int getSenderTerm() const override { return _sender_term; }
        int getSenderId() const override { return _sender_id; }
        int getSenderLastLogIndex() const override { return _sender_last_log_index; }
        int getSenderLastLogTerm() const override { return _sender_last_log_trem; }

    protected:
        RaftRequest(int sender_term, int sender_id, int sender_last_log_index,int sender_last_log_term, Kind rkind);
    private:
        int _sender_term, _sender_id, _sender_last_log_index,_sender_last_log_trem;
    };

/*****************************************************************************
************************* Section 3: Concrete Request ************************
*****************************************************************************/
// Note: This section defines specific classes that can be created by the user


/************************ Classes created by clients ************************/

    class Register : public UserRequest
    {
    public:
        Register(const string &username, const string &password);
        bool hasAttribute(Attribute atr) const override;
        string getPassword() const override { return _password; }

        Request *clone() const override { return new Register(*this); }
    private:
        string _password;
    };

    class SignIn:public UserRequest
    {
    public:
        SignIn(const string &username, const string &password);
        bool hasAttribute(Attribute) const override;
        string getPassword() const override { return _password; }

        Request *clone() const override { return new SignIn(*this); }
    private:
        string _password;
    };

    class Deal:public UserRequest
    {
    public:
        Deal(const string &username, const string &dest, double volume,const Date &date);
        bool hasAttribute(Attribute atr) const override;

        string getAccountCollector() const override { return _account_collector; }
        double getAccount() const override { return _volume; }
        Date getTransactionDate() const override { return _date; }

        Request *clone() const override { return new Deal(*this); }
    private:
        string _account_collector;
        double _volume;
        Date _date;
    };

    class BalanceCheck:public UserRequest
    {
    public:
        BalanceCheck(const string &username);
        Request *clone() const override { return new BalanceCheck(*this); }
    };

    class RecordCheck:public UserRequest
    {
    public:
        RecordCheck(const string &username, const Date &start, const Date &end);
        bool hasAttribute(Attribute) const override;
        Date getStartDate() const override { return _start; }
        Date getEndDate() const override { return _end; }

        Request *clone() const override { return new RecordCheck(*this); }
    private:
        Date _start, _end;
    };

/************************** Classes created by Raft **************************/
    class Vote:public RaftRequest
    {
    public:
        Vote(int candidate_term, int candidate_id, int lastLogIndex, int lastLogTerm);
        Request *clone() const override { return new Vote(*this); }
    };

    class Addition:public RaftRequest
    {
    public:
        template <typename It>
        Addition(int leader_term,
                 int leader_id,
                 int prevLogIndex,
                 int prevLogTerm,
                 int leader_commit,
                 It begin,
                 It end);

        bool hasAttribute(Attribute atr) const override;
        vector<string> getEntries() const override { return _entries; }
        int getLeaderCommit() const override { return _leader_commit; }

        Request *clone() const override { return new Addition(*this); }
    private:
        vector<string> _entries;
        int _leader_commit;
    };    

template <typename It>
Addition::Addition(int leader_term,int leader_id,
                   int prevLogIndex, int prevLogTerm,
                   int leader_commit,
                   It begin, It end)
            : RaftRequest(leader_term, leader_id, prevLogIndex, prevLogTerm, Kind::Addition),
              _leader_commit(leader_commit)
{
    std::copy(begin, end, std::back_inserter(_entries));
}
/************************** Undetermined request ******************************/
/*
    class Transmit : public Request
    {
    public:
        Transmit(const string &forwarder,const Request &req);
        bool hasAttribute(Attribute) const override;

        string getForwarder() const { return _forwarder; }
        shared_ptr<Request> getRequest() const { return _request; }

        Request *clone() const override { return new Transmit(*this); }
    private:
        string _forwarder;
        shared_ptr<Request> _request;
    };
    */
} // namespace request
