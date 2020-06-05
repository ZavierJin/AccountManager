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

bool Raft::acceptVote()
{
	Vote vote;
	bool voteGranted = false;
	bool is_change_role = false;
	TermType my_last_log_term;
	auto& com = computer::Computer::instance();

	auto req = com.getRequest(request::Kind::Vote);//获得request 
	if (req == nullptr) return false;
	
	vote.term = req->getSenderTerm();//接收各个数据////////////////////
	vote.candidateId = req->getSenderId();
	vote.lastLogIndex = req->getSenderLastLogIndex();
	vote.lastLogTerm = req->getSenderLastLogTerm();

	//my_last_log_term = 0;
	my_last_log_term = logs.getTerm(logs.getNewest()); //获得最新日志的term/////用函数读////////

	if (vote.term > currentTerm) //收到的任期大于自己的情况
	{
		setTerm(vote.term);//改变term 
		is_change_role = (nodeState != FOLLOWER) ? true : false;
		changeRole(FOLLOWER); //变成follower////////////////////////////////////////
		//raftTimer.Reset(FOLLOWER);//更新倒计时////////////////////////////////////////////////////
#ifdef RAFT_SHOW
		writeSaid("Term problem, change to Follower");
#endif // RAFT_SHOW
	}
	else
	{
		//raftTimer.Reset(nodeState);//更新倒计时//////////////////////////////////////////////			
	}

	if(vote.term < currentTerm                     //否定票情况
		|| (votedFor != vote.candidateId && votedFor != INVALID_ID)	////////////	
		|| vote.lastLogTerm < my_last_log_term
		|| vote.lastLogTerm == my_last_log_term && vote.lastLogIndex < commitIndex)
	{
		voteGranted = false;
#ifdef RAFT_SHOW
		writeSaid("Refuse to vote Node_" + std::to_string(vote.candidateId));
#endif // RAFT_SHOW
	}
	else   //肯定票情况
	{
		votedFor = vote.candidateId;
		voteGranted = true;
#ifdef RAFT_SHOW
		writeSaid("Agree to vote Node_" + std::to_string(vote.candidateId));
#endif // RAFT_SHOW
	}

	answer::VoteAnswer  ans(currentTerm, voteGranted);
	com.sendAnswer(ans, req->getAddress());// 给candidateId 发送投票反馈
#ifdef RAFT_DEBUG
	writeSaid("Answer vote to Node_" + std::to_string(vote.candidateId));
#endif // RAFT_DEBUG

	return is_change_role;
}




bool Raft::acceptVote_request()
{
	if (nodeState != CANDIDATE) return false;

	TermType receiver_term = INITIAL_TERM;
	bool voteGranted = false;
	bool is_change_role = false;
	auto& com = computer::Computer::instance();

	// discard the remaining AdditionAnswer
	discardAdditionAnswer();	// not necessary

	auto req = com.getAnswer(answer::Kind::VoteAnswer);
	if (req == nullptr)  return false;

	voteGranted = req->getAttitude();
	receiver_term = req->getReceiverTerm();

	// compare term!!!
	if (receiver_term > currentTerm) //收到的任期大于自己的情况
	{
		setTerm(receiver_term);//改变term 
		is_change_role = true;
		changeRole(FOLLOWER); //变成follower////////////////////////////////////////
		//raftTimer.Reset(FOLLOWER);//更新倒计时////////////////////////////////////////////////////
#ifdef RAFT_SHOW
		writeSaid("Term problem, from Candidtate to Follower");
#endif // RAFT_SHOW
	}


	if (voteGranted == true)
	{
		votedTotal++;
#ifdef RAFT_SHOW
		writeSaid("Got support, votedTotal[" + std::to_string(votedTotal) + "]");
#endif // RAFT_SHOW
	}

	if (votedTotal > nodeTotal / 2)		// >= to > ??
	{
		is_change_role = true;
		changeRole(LEADER);//////转换成leader///////////////////////////////////////////////////////
		//raftTimer.Reset(LEADER);//更新倒计时////////////////////////////////////////////////////
		// LeaderID = nodeId; // 需要加一个全局变量	!!!!
		leaderId = nodeId; // Raft自己的leaderid改变 
#ifdef RAFT_SHOW
		writeSaid("Change to leader");
#endif // RAFT_SHOW

		
		// discard the remaining VoteAnswer
		discardVoteAnswer();	// not necessary
	}
	return is_change_role;
}