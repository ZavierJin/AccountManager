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
	} while (nodeIdList.size() != nodeTotal);	// wait until everyone is ready
	raftTimer.startCountDown();		// Make sure everyone start at the same time
	resetLeaderPara();
	fileWriter.open(std::to_string(nodeId));
#ifdef SHOW
	std::cout << "What a wonderful world!" << std::endl;
	writeSaid("Hello, I'm Raft-" + std::to_string(nodeId));

	std::string str_node_id_list("NodeIdList: ");
	for (auto i : nodeIdList) {
		str_node_id_list += std::to_string(i) + " ";
	}
	writeSaid(str_node_id_list);
#endif // SHOW
}

// other
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
		votedFor = nodeId;
		raftTimer.Reset(CANDIDATE);
		votedTotal = 1;
		showMyInfo();
		sendVote();
		break;
	case LEADER:
		resetLeaderPara();
		raftTimer.Reset(LEADER);
		showMyInfo();
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
	//f_out << "[" << raftTimer.getTrueTime() << "] " << "electionTimeOut" << std::endl;
	bool is_time_out = raftTimer.Timeout();
	if (is_time_out) {
		setTerm(currentTerm + 1);
		changeRole(CANDIDATE);
#ifdef DEBUG
		std::cout << "I'm a Candidate now!!" << std::endl;
#endif // DEBUG
	}
	return is_time_out;
}

void Raft::showMyInfo()
{
#ifdef SHOW
	std::string str_state;
	switch (nodeState)
	{
	case FOLLOWER:	str_state =  "Follower"; break;
	case CANDIDATE:	str_state = "Candidate"; break;
	case LEADER:	str_state = "Leader"; break;
	default:		str_state = "Stranger"; break;
	}
	writeSaid("I'm a " + str_state + " now, term[" 
		+ std::to_string(currentTerm) + "]");
#endif // SHOW
}

void Raft::writeSaid(const std::string& raft_said)
{
	fileWriter << "[" << raftTimer.getTrueTime() << "] ";
	fileWriter << raft_said << std::endl;
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
