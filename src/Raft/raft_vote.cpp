/* raft_vote.h
** Brief introduction: the implementation of Raft's method about vote
** Create by Jiang ChenXing
** Modify:
**      2020/6/3 (by Jin Zhanyu):
**			1. Define local variables: my_last_log_term & voteGranted
**			2. Correct some writing mistakes
**			3. request::Kind::Vote
**			4. remove Reset to function ChangeRole
**			5. add bool return, is_change_role
**			6. add getAnswer in acceptVote_request
*/

#include "Raft.h"

//using f_out;
using std::cout;
using std::endl;

bool Raft::acceptVote()
{
	f_out << "acceptVote" << std::endl;
	Vote vote;
	bool voteGranted = false;
	bool is_change_role = false;
	TermType my_last_log_term;
	auto& com = computer::Computer::instance();

#ifdef DEBUG
	//f_out << "Try get request!! " << std::endl;
#endif // DEBUG
	auto req = com.getRequest(request::Kind::Vote);//获得request 
	if (req == nullptr) return false;
	
#ifdef DEBUG
	f_out << "Get request!! " << std::endl;
#endif // DEBUG
	vote.term = req->getSenderTerm();//接收各个数据////////////////////
	vote.candidateId = req->getSenderId();
	vote.lastLogIndex = req->getSenderLastLogIndex();
	vote.lastLogTerm = req->getSenderLastLogTerm();

	my_last_log_term = 0;
	//my_last_log_term = logs.getTerm(logs.getNewest()); //获得最新日志的term/////用函数读////////

	if (vote.term > currentTerm) //收到的任期大于自己的情况
	{
		setTerm(vote.term);//改变term 
		is_change_role = (nodeState != FOLLOWER) ? true : false;
		changeRole(FOLLOWER); //变成follower////////////////////////////////////////
		//raftTimer.Reset(FOLLOWER);//更新倒计时////////////////////////////////////////////////////
		cout << "收到的任期大于自己,改变自己的任期,并变成follower" << endl;//debug
	}
	else
	{
		//raftTimer.Reset(nodeState);//更新倒计时//////////////////////////////////////////////			
	}

	if(vote.term < currentTerm                     //否定票情况
		|| (votedFor != vote.candidateId && votedFor != INVALID_ID))	////////////	
		//|| vote.lastLogTerm < my_last_log_term
		//|| vote.lastLogTerm == my_last_log_term && vote.lastLogIndex < commitIndex)
	{
		voteGranted = false;
#ifdef SHOW
		f_out << "Reject to vote " << "Node_" << vote.candidateId << std::endl;
#endif // SHOW
	}
	else   //肯定票情况
	{
		votedFor = vote.candidateId;
		voteGranted = true;
#ifdef SHOW
		f_out << "Agree to vote " << "Node_" << vote.candidateId << std::endl;
#endif // SHOW
	}

	answer::VoteAnswer  ans(currentTerm, voteGranted);
	com.sendAnswer(ans, req->getAddress());// 给candidateId 发送投票反馈
#ifdef SHOW
	f_out << "Answer vote to " << "Node_" << vote.candidateId << std::endl;
#endif // SHOW

	return is_change_role;
}

bool Raft::acceptVote_request()
{
	f_out << "acceptVote_request" << std::endl;
	if (nodeState != CANDIDATE) return false;

	TermType receiver_term = INITIAL_TERM;
	bool voteGranted = false;
	bool is_change_role = false;
	auto& com = computer::Computer::instance();

#ifdef DEBUG
	//f_out << "Try get answer!! " << std::endl;
#endif // DEBUG
	// getAnswer!!
	auto req = com.getAnswer(answer::Kind::VoteAnswer);
	if (req == nullptr)  return false;
	
#ifdef DEBUG
	f_out << "Get answer!! " << std::endl;
#endif // DEBUG
	voteGranted = req->getAttitude();
	receiver_term = req->getReceiverTerm();

	// compare term!!!
	if (receiver_term > currentTerm) //收到的任期大于自己的情况
	{
		setTerm(receiver_term);//改变term 
		is_change_role = true;
		changeRole(FOLLOWER); //变成follower////////////////////////////////////////
		//raftTimer.Reset(FOLLOWER);//更新倒计时////////////////////////////////////////////////////
		cout << "收到的任期大于自己,改变自己的任期,并变成follower" << endl;//debug
	}


	if (voteGranted == true)
	{
		votedTotal++;
		cout << "votalTotal++" << endl;//debug
	}

	if (votedTotal > nodeTotal / 2)		// >= to > ??
	{
		is_change_role = true;
		changeRole(LEADER);//////转换成leader///////////////////////////////////////////////////////
		//raftTimer.Reset(LEADER);//更新倒计时////////////////////////////////////////////////////
		// LeaderID = nodeId; // 需要加一个全局变量 
		leaderId = nodeId; // Raft自己的leaderid改变 
		cout << "转变成leader" << endl;//debug

		/*
		// discard others
		while (com.hasAnswer(answer::Kind::VoteAnswer)) {
			auto discard = com.getAnswer(answer::Kind::VoteAnswer);
#ifdef DEBUG
			f_out << "Discard!!! " << std::endl;
#endif // DEBUG
		}
		*/
	}
	return is_change_role;
}