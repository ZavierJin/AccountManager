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
*/
#include "Raft.h"


void Raft::acceptAppendEntries()
{
	AppendEntries		appendEntries;
	AppendEntriesReply	appendEntriesReply;
	auto& com = computer::Computer::instance();

	if (!com.hasRequest(request::Kind::Addition)) return;

	auto req = com.getRequest(request::Kind::Addition);
	//appendEntries.entries = req->getEntries();
	appendEntries.leaderCommit = req->getLeaderCommit();
	appendEntries.leaderId = req->getSenderId();
	appendEntries.prevLogTerm = req->getSenderLastLogTerm();
	appendEntries.prevLogIndex = req->getSenderLastLogIndex();
	appendEntries.term = req->getSenderTerm();

	appendEntriesReply.nodeId = nodeId;

	if ((nodeState == CANDIDATE || nodeState == LEADER) && appendEntries.term >= currentTerm) {
		setTerm(appendEntries.term);			// use setTerm
		appendEntriesReply.term = currentTerm;
		changeRole(FOLLOWER);
	}

	if (appendEntries.term < currentTerm) {
		appendEntriesReply.success = false;
		appendEntriesReply.term = currentTerm;
		return;
	}
	/*
	if (logs.getTerm(appendEntries.prevLogIndex) != appendEntries.prevLogTerm) {
		appendEntriesReply.success = false;
		appendEntriesReply.term = currentTerm;
		return;
	}                          //log维护一个初始0
	
	logs.deleteLogs(appendEntries.prevLogIndex);
	*/

	/* ??
	for (auto it = appendEntries.entries.begin(); it != appendEntries.entries.end(); ++it) {
		logs.addLog(*it);
	}
	*/

	if (appendEntries.leaderCommit > commitIndex) {
		commitIndex = appendEntries.leaderCommit;             //最新日志中较小的那一个
	}
	/*
	if (commitIndex > lastApplied) {
		apply(logs[lastApplied - commitIndex]);                 //账本仍未给,测试时可注去
	}
	*/

	appendEntriesReply.success = true;
	appendEntriesReply.term = currentTerm;
	/*
	answer::AdditionAnswer	ans(appendEntriesReply.success,
								appendEntriesReply.term, 
								appendEntriesReply.nodeId);
	com.sendAnswer(ans, req->getAddress());
	*/
	std::cout << "Send AppendEntries Over!" << std::endl;
}

void Raft::acceptAppendEntriesReply()
{
	/*
	AppendEntriesReply	appendEntriesReply;
	auto& com = computer::Computer::instance();

	if (com.hasAnswer(answer::Kind::AdditionAnswer)) {
		auto req = com.getAnswer(answer::Kind::AdditionAnswer);
		appendEntriesReply.nodeId = req->getReceiverId();
		appendEntriesReply.success = req->getAttitude();
		appendEntriesReply.term = req->getReceiverTerm();
	}
	else return;

	if (appendEntriesReply.success) {
		nextIndex[appendEntriesReply.nodeId] = logs.getNewest() + 1;
		if (matchIndex[appendEntriesReply.nodeId] != logs.getNewest()) {
			matchIndex[appendEntriesReply.nodeId] = logs.getNewest();
			matchTotal++;
		}
		if (matchTotal > nodeTotal / 2) {
			commitIndex = logs.getNewest();
			matchTotal = 0;
			// apply(log[lastapplied - commitIndex]);        //账本未给，测试可注去
		}
	}

	if (!appendEntriesReply.success) {
		if (currentTerm < appendEntriesReply.term) {
			setTerm(appendEntriesReply.term);		// use setTerm
			changeRole(FOLLOWER);
			return;
		}
		if (nextIndex[appendEntriesReply.nodeId] > 1) {
			nextIndex[appendEntriesReply.nodeId]--;
		}
	}
	*/
	std::cout << "Accept AppendEntries Over!" << std::endl;
}