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

//using f_out << "[" << raftTimer.getTrueTime() << "] ";


bool Raft::acceptVote()
{
	//f_out << "[" << raftTimer.getTrueTime() << "] " << "acceptVote" << std::endl;
	//auto& com = computer::Computer::instance();
	//auto req = com.getRequest(request::Kind::Vote);//���request 
	//if (req == nullptr)  return false;
	//	//auto req = com.getRequest(request::Kind::Vote);//���request 

	//f_out << "[" << raftTimer.getTrueTime() << "] " << "Get request!! " << std::endl;
	//return false;
	Vote vote;
	bool voteGranted = false;
	bool is_change_role = false;
	TermType my_last_log_term;
	auto& com = computer::Computer::instance();

#ifdef RAFT_DEBUG
	//f_out << "[" << raftTimer.getTrueTime() << "] " << "Try get request!! " << std::endl;
#endif // RAFT_DEBUG
	auto req = com.getRequest(request::Kind::Vote);//���request 
	if (req == nullptr) return false;
	
#ifdef RAFT_DEBUG
	//f_out << "[" << raftTimer.getTrueTime() << "] " << "Get request!! " << std::endl;
#endif // RAFT_DEBUG
	vote.term = req->getSenderTerm();//���ո�������////////////////////
	vote.candidateId = req->getSenderId();
	vote.lastLogIndex = req->getSenderLastLogIndex();
	vote.lastLogTerm = req->getSenderLastLogTerm();

	my_last_log_term = 0;
	//my_last_log_term = logs.getTerm(logs.getNewest()); //���������־��term/////�ú�����////////

	if (vote.term > currentTerm) //�յ������ڴ����Լ������
	{
		setTerm(vote.term);//�ı�term 
		is_change_role = (nodeState != FOLLOWER) ? true : false;
		changeRole(FOLLOWER); //���follower////////////////////////////////////////
		//raftTimer.Reset(FOLLOWER);//���µ���ʱ////////////////////////////////////////////////////
#ifdef RAFT_SHOW
		writeSaid("Term problem, change to Follower");
#endif // RAFT_SHOW
	}
	else
	{
		//raftTimer.Reset(nodeState);//���µ���ʱ//////////////////////////////////////////////			
	}

	if(vote.term < currentTerm                     //��Ʊ���
		|| (votedFor != vote.candidateId && votedFor != INVALID_ID))	////////////	
		//|| vote.lastLogTerm < my_last_log_term
		//|| vote.lastLogTerm == my_last_log_term && vote.lastLogIndex < commitIndex)
	{
		voteGranted = false;
#ifdef RAFT_SHOW
		writeSaid("Refuse to vote Node_" + std::to_string(vote.candidateId));
#endif // RAFT_SHOW
	}
	else   //�϶�Ʊ���
	{
		votedFor = vote.candidateId;
		voteGranted = true;
#ifdef RAFT_SHOW
		writeSaid("Agree to vote Node_" + std::to_string(vote.candidateId));
#endif // RAFT_SHOW
	}

	answer::VoteAnswer  ans(currentTerm, voteGranted);
	com.sendAnswer(ans, req->getAddress());// ��candidateId ����ͶƱ����
#ifdef RAFT_DEBUG
	writeSaid("Answer vote to Node_" + std::to_string(vote.candidateId));
#endif // RAFT_DEBUG

	return is_change_role;
}




bool Raft::acceptVote_request()
{
	//f_out << "[" << raftTimer.getTrueTime() << "] " << "acceptVote_request" << std::endl;
	if (nodeState != CANDIDATE) return false;

	TermType receiver_term = INITIAL_TERM;
	bool voteGranted = false;
	bool is_change_role = false;
	auto& com = computer::Computer::instance();

#ifdef RAFT_DEBUG
	//f_out << "[" << raftTimer.getTrueTime() << "] " << "Try get answer!! " << std::endl;
#endif // RAFT_DEBUG
	// getAnswer!!
	auto req = com.getAnswer(answer::Kind::VoteAnswer);
	if (req == nullptr)  return false;
	
#ifdef RAFT_DEBUG
	//f_out << "[" << raftTimer.getTrueTime() << "] " << "Get answer!! " << std::endl;
#endif // RAFT_DEBUG
	voteGranted = req->getAttitude();
	receiver_term = req->getReceiverTerm();

	// compare term!!!
	if (receiver_term > currentTerm) //�յ������ڴ����Լ������
	{
		setTerm(receiver_term);//�ı�term 
		is_change_role = true;
		changeRole(FOLLOWER); //���follower////////////////////////////////////////
		//raftTimer.Reset(FOLLOWER);//���µ���ʱ////////////////////////////////////////////////////
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
		changeRole(LEADER);//////ת����leader///////////////////////////////////////////////////////
		//raftTimer.Reset(LEADER);//���µ���ʱ////////////////////////////////////////////////////
		// LeaderID = nodeId; // ��Ҫ��һ��ȫ�ֱ��� 
		leaderId = nodeId; // Raft�Լ���leaderid�ı� 
#ifdef RAFT_SHOW
		writeSaid("Change to leader");
#endif // RAFT_SHOW

		
		// discard the remaining VoteAnswer
		while (com.hasAnswer(answer::Kind::VoteAnswer)) {
			auto discard = com.getAnswer(answer::Kind::VoteAnswer);
#ifdef RAFT_DEBUG
			writeSaid("Discard the remaining VoteAnswer!!! ");
#endif // RAFT_DEBUG
		}
		
	}
	return is_change_role;
}