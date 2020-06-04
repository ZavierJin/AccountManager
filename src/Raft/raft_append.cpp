/* raft_append.h
** Brief introduction: the implementation of Raft's method about append entries
** Create by Zhang Zhiyu
** Modify:
**      2020/6/3 (by Jin Zhanyu):
**			1. Define local variables: appendEntries & appendEntriesReply
**			2. Correct some writing mistakes
**			3. Modify first if 
**			4. request::Kind::Addition  &  answer::Kind::AdditionAnswer
**			5. pass appendEntriesReply.nodeId for AdditionAnswer in line 72 
**      2020/6/4 (by Jin Zhanyu):
**			1. add bool return, is_change_role
**			2. reset election time out
*/
#include "Raft.h"


bool Raft::acceptAppendEntries()
{
	AppendEntries		appendEntries;
	AppendEntriesReply	appendEntriesReply;
	bool is_change_role = false;
	auto& com = computer::Computer::instance();

	auto req = com.getRequest(request::Kind::Addition);
	if (req == nullptr) return false;
	
	appendEntries.entries = req->getEntries();
	appendEntries.leaderCommit = req->getLeaderCommit();
	appendEntries.leaderId = req->getSenderId();
	appendEntries.prevLogTerm = req->getSenderLastLogTerm();
	appendEntries.prevLogIndex = req->getSenderLastLogIndex();
	appendEntries.term = req->getSenderTerm();

	appendEntriesReply.nodeId = nodeId;

	if ((nodeState == CANDIDATE || nodeState == LEADER) && appendEntries.term >= currentTerm) {
		setTerm(appendEntries.term);			// use setTerm
		appendEntriesReply.term = currentTerm;
		is_change_role = (nodeState != FOLLOWER) ? true : false;
		changeRole(FOLLOWER);
#ifdef RAFT_SHOW
		writeSaid("Term problem, change to Follower");
#endif // RAFT_SHOW
	}

	if (appendEntries.term < currentTerm) {
#ifdef RAFT_SHOW
		writeSaid("Refuse(1) to append Node_" + std::to_string(appendEntries.leaderId));
#endif // RAFT_SHOW
		appendEntriesReply.success = false;
		appendEntriesReply.term = currentTerm;
		return is_change_role;
	}
	
	if (logs.getTerm(appendEntries.prevLogIndex) != appendEntries.prevLogTerm) {
#ifdef RAFT_SHOW
		writeSaid("Refuse(2) to append Node_" + std::to_string(appendEntries.leaderId));
		writeSaid("getTerm: " + std::to_string(logs.getTerm(appendEntries.prevLogIndex)));
		writeSaid("leader prevLogTerm: " + std::to_string(appendEntries.prevLogTerm));
		writeSaid("leader prevLogIndex[ " + std::to_string(appendEntries.prevLogIndex));
#endif // RAFT_SHOW
		appendEntriesReply.success = false;
		appendEntriesReply.term = currentTerm;
		return is_change_role;
	}                          //log维护一个初始0
	
	// update logs
	logs.deleteLogs(appendEntries.prevLogIndex + 1);	//appendEntries.prevLogIndex
	for (auto one_log : appendEntries.entries) {
		logs.addLog(one_log);
	}
	

	if (appendEntries.leaderCommit > commitIndex) {
		commitIndex = appendEntries.leaderCommit;             //最新日志中较小的那一个
#ifdef RAFT_SHOW
		writeSaid("Update commitIndex[" + std::to_string(commitIndex) + "]");
#endif // RAFT_SHOW
	}
	
	if (commitIndex > lastApplied) {
		//apply(logs[lastApplied - commitIndex]);                 //账本仍未给,测试时可注去
	}
	
#ifdef RAFT_SHOW
	writeSaid("Agree to append Node_" + std::to_string(appendEntries.leaderId));
#endif // RAFT_SHOW
	appendEntriesReply.success = true;
	appendEntriesReply.term = currentTerm;
	raftTimer.Reset(FOLLOWER);		// add reset
	
	answer::AdditionAnswer	ans(appendEntriesReply.success,
								appendEntriesReply.term, 
								appendEntriesReply.nodeId);
	com.sendAnswer(ans, req->getAddress());
#ifdef RAFT_DEBUG
	writeSaid("Answer append to Node_" + std::to_string(appendEntries.leaderId));
#endif // RAFT_DEBUG

	return is_change_role;
}

bool Raft::acceptAppendEntriesReply()
{
	if (nodeState != LEADER) return false;

	bool is_change_role = false;
	AppendEntriesReply	appendEntriesReply;
	auto& com = computer::Computer::instance();

	auto req = com.getAnswer(answer::Kind::AdditionAnswer);
	if (req == nullptr)  return false;

#ifdef RAFT_DEBUG
	writeSaid("Get apppend answer.");
#endif // RAFT_DEBUG
		
	appendEntriesReply.nodeId = req->getReceiverId();
	appendEntriesReply.success = req->getAttitude();
	appendEntriesReply.term = req->getReceiverTerm();

	
	if (appendEntriesReply.success) {
		nextIndex[appendEntriesReply.nodeId] = logs.getNewest() + 1;
		if (matchIndex[appendEntriesReply.nodeId] != logs.getNewest()) {
			matchIndex[appendEntriesReply.nodeId] = logs.getNewest();
			matchTotal++;
#ifdef RAFT_SHOW
			writeSaid("Got agreement, matchTotal[" + std::to_string(matchTotal) + "]");
#endif // RAFT_SHOW
		}
		if (matchTotal > nodeTotal / 2) {
			commitIndex = logs.getNewest();
			matchTotal = 0;
#ifdef RAFT_SHOW
			writeSaid("Update commitIndex[" + std::to_string(commitIndex) + "]");
#endif // RAFT_SHOW
			// apply(log[lastapplied - commitIndex]);        //账本未给，测试可注去
#ifdef RAFT_DEBUG
			writeSaid("Write into account???");
#endif // RAFT_DEBUG

			// discard the remaining AdditionAnswer
			while (com.hasAnswer(answer::Kind::AdditionAnswer)) {
				auto discard = com.getAnswer(answer::Kind::AdditionAnswer);
#ifdef RAFT_DEBUG
				writeSaid("Discard the remaining AdditionAnswer!!! ");
#endif // RAFT_DEBUG
			}
		}
	}

	if (!appendEntriesReply.success) {
		if (currentTerm < appendEntriesReply.term) {
#ifdef RAFT_SHOW
			writeSaid("Term problem, from Leader to Follower");
#endif // RAFT_SHOW
			setTerm(appendEntriesReply.term);		// use setTerm
			is_change_role = true;
			changeRole(FOLLOWER);
			return is_change_role;
		}
		if (nextIndex[appendEntriesReply.nodeId] > 1) {
			nextIndex[appendEntriesReply.nodeId]--;
		}
	}

	return is_change_role;
}