/* Raft.cpp
** Brief introduction: the implementation of Raft's other methods
** Create by Jin Zhanyu 2020/6/3
*/

#include "Raft.h"

Raft::Raft() :raftTimer(INITIAL_STATE)
{
	auto& com = computer::Computer::instance();
	nodeId = com.registerServer();
	do {
		nodeIdList = com.getOnlineServer();
	} while (nodeIdList.size() != nodeTotal);	// wait
	resetLeaderPara();
#ifdef DEBUG
	f_out << "Hello, I'm Raft-" << nodeId << std::endl;
	std::cout << "What a wonderful world!" << std::endl;
	std::cout << "NodeIdList: ";
	for (auto i : nodeIdList) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
#endif // DEBUG
}

// other
void Raft::changeRole(StateType nowState)
{
	nodeState = nowState;
	switch (nodeState)
	{
	case FOLLOWER:
		raftTimer.Reset(FOLLOWER);
		break;
	case CANDIDATE:
		setTerm(currentTerm + 1);
		votedFor = nodeId;
		raftTimer.Reset(CANDIDATE);
		votedTotal = 1;
		sendVote();
		break;
	case LEADER:
		resetLeaderPara();
		raftTimer.Reset(LEADER);
		break;
	default:
		break;
	}
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
	f_out << "electionTimeOut" << std::endl;
	bool is_time_out = raftTimer.Timeout();
	if (is_time_out) {
#ifdef DEBUG
		std::cout << "I'm a Candidate now!!" << std::endl;
#endif // DEBUG
		changeRole(CANDIDATE);
	}
	return is_time_out;
}

// Interaction with Examiner
void Raft::receiveExaminer()
{
	// if (/* have msg */) {
		/* read */
		/* add into logs */
	// }
}

void Raft::answerExaminer() const
{
	/* answer */
}
