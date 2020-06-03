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
	auto req = com.getRequest(request::Kind::Vote);//���request 
	if (req == nullptr) return false;
	
#ifdef DEBUG
	f_out << "Get request!! " << std::endl;
#endif // DEBUG
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
		cout << "�յ������ڴ����Լ�,�ı��Լ�������,�����follower" << endl;//debug
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
#ifdef SHOW
		f_out << "Reject to vote " << "Node_" << vote.candidateId << std::endl;
#endif // SHOW
	}
	else   //�϶�Ʊ���
	{
		votedFor = vote.candidateId;
		voteGranted = true;
#ifdef SHOW
		f_out << "Agree to vote " << "Node_" << vote.candidateId << std::endl;
#endif // SHOW
	}

	answer::VoteAnswer  ans(currentTerm, voteGranted);
	com.sendAnswer(ans, req->getAddress());// ��candidateId ����ͶƱ����
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
	if (receiver_term > currentTerm) //�յ������ڴ����Լ������
	{
		setTerm(receiver_term);//�ı�term 
		is_change_role = true;
		changeRole(FOLLOWER); //���follower////////////////////////////////////////
		//raftTimer.Reset(FOLLOWER);//���µ���ʱ////////////////////////////////////////////////////
		cout << "�յ������ڴ����Լ�,�ı��Լ�������,�����follower" << endl;//debug
	}


	if (voteGranted == true)
	{
		votedTotal++;
		cout << "votalTotal++" << endl;//debug
	}

	if (votedTotal > nodeTotal / 2)		// >= to > ??
	{
		is_change_role = true;
		changeRole(LEADER);//////ת����leader///////////////////////////////////////////////////////
		//raftTimer.Reset(LEADER);//���µ���ʱ////////////////////////////////////////////////////
		// LeaderID = nodeId; // ��Ҫ��һ��ȫ�ֱ��� 
		leaderId = nodeId; // Raft�Լ���leaderid�ı� 
		cout << "ת���leader" << endl;//debug

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