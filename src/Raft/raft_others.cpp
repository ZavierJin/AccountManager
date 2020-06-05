/* Raft.cpp
** Brief introduction: the implementation of Raft's other methods
** Create by Jin Zhanyu 2020/6/3
*/

#include "Raft.h"

Raft::Raft() :raftTimer(INITIAL_STATE)
{
	auto& com = computer::Computer::instance();
	nodeId = com.registerServer();			// get nodeId
	do {
		nodeIdList = com.getOnlineServer();
	} while (nodeIdList.size() != nodeTotal);	// wait until everyone is ready
	raftTimer.startCountDown();		// Make sure everyone start at the same time
	resetLeaderPara();
	fileWriter.open(std::to_string(nodeId));
#ifdef RAFT_SHOW
	std::cout << "What a wonderful world!" << std::endl;
	writeSaid("Hello, I'm Raft-" + std::to_string(nodeId));

	std::string str_node_id_list("NodeIdList: ");
	for (auto i : nodeIdList) {
		str_node_id_list += std::to_string(i) + " ";
	}
	writeSaid(str_node_id_list);
#endif // RAFT_SHOW
}

// change role & reset Timer
void Raft::changeRole(StateType nowState)
{
	nodeState = nowState;
	switch (nodeState)
	{
	case FOLLOWER:
		raftTimer.Reset(FOLLOWER);
		showMyInfo();
		break;
	case CANDIDATE:
		raftTimer.Reset(CANDIDATE);
		votedFor = nodeId;
		votedTotal = 1;		// vote for myself
		showMyInfo();
		sendVote();			// send vote request
		break;
	case LEADER:
		raftTimer.Reset(LEADER);
		resetLeaderPara();
		showMyInfo();
		break;
	default:
		break;
	}
}

// set term & reset votedFor
void Raft::setTerm(TermType new_term)
{
	currentTerm = new_term;
	votedFor = INVALID_ID;      // reset for new term
}

void Raft::resetLeaderPara()
{
	for (auto i : nodeIdList) {
		matchIndex[i] = 0;
		nextIndex[i] = commitIndex + 1;
	}
}

bool Raft::electionTimeOut()
{
	if (nodeState == LEADER) return false;

	bool is_time_out = raftTimer.Timeout();
	if (is_time_out) {
		setTerm(currentTerm + 1);
		changeRole(CANDIDATE);
	}
	return is_time_out;
}

bool Raft::beatTimeOut()
{
	if (nodeState != LEADER) return false;

	bool is_time_out = raftTimer.Timeout();
	if (is_time_out) {
		raftTimer.Reset(LEADER);
		sendAppendEntries();
	}
	return is_time_out;
}

void Raft::showMyInfo()
{
#ifdef RAFT_SHOW
	std::string str_state;
	switch (nodeState)
	{
	case FOLLOWER:	str_state = "Follower"; break;
	case CANDIDATE:	str_state = "Candidate"; break;
	case LEADER:	str_state = "Leader"; break;
	default:		str_state = "Stranger"; break;
	}
	writeSaid("I'm a " + str_state + " now, term[" 
		+ std::to_string(currentTerm) + "]");
	std::cout << "I'm a " << str_state << " now, term[" 
		<< currentTerm << "]" << std::endl;
#endif // RAFT_SHOW
}

void Raft::writeSaid(const std::string& raft_said)
{
	fileWriter << "[" << raftTimer.getTrueTime() << "] ";
	fileWriter << raft_said << std::endl;
}

void Raft::discardVoteAnswer()
{
	auto& com = computer::Computer::instance();
	// discard the remaining VoteAnswer
	while (com.hasAnswer(answer::Kind::VoteAnswer)) {
		auto discard = com.getAnswer(answer::Kind::VoteAnswer);
#ifdef RAFT_DEBUG
		writeSaid("Discard the remaining VoteAnswer!!! ");
#endif // RAFT_DEBUG
	}
}

void Raft::discardAdditionAnswer()
{
	auto& com = computer::Computer::instance();
	// discard the remaining AdditionAnswer
	while (com.hasAnswer(answer::Kind::AdditionAnswer)) {
		auto discard = com.getAnswer(answer::Kind::AdditionAnswer);
#ifdef RAFT_DEBUG
		writeSaid("Discard the remaining AdditionAnswer!!! ");
#endif // RAFT_DEBUG
	}
}

// Interaction with Examiner
void Raft::receiveExaminer()
{
	if (nodeState != LEADER) return;

	std::string action;
#ifdef RAFT_DEBUG
	static bool has_msg = true;
	static IndexType pre_index = commitIndex;
	static long count = 0;
	if (commitIndex < 4 && pre_index < commitIndex) {
		count++;
		if (count > 3000)
			has_msg = true;
	}
#endif // RAFT_DEBUG
	//bool has_msg = false;
	if (has_msg) {/* have msg */
		/* read */
		count = 0;	// debug
		has_msg = false;
		action = "Learning C++ is interesting?";
		/* add into logs */
		logs.addAction(action, currentTerm);
#ifdef RAFT_SHOW
		writeSaid("Receive action from Examiner.");
#endif // RAFT_SHOW
	}
}

void Raft::answerExaminer() const
{
	/* answer */
}
