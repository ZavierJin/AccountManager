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
**      2020/6/5 (by Jin Zhanyu):
**			1. modify if logic, answer rejection to leader
**      2020/6/6 (by Jin Zhanyu):
**			1. delete discardAdditionAnswer();
**		2020/6/9(by Jiangpin):
**			1.add applyAppendEntries()
*/
#include "Raft.h"
#include "check.h"

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

	// reject
	if (appendEntries.term < currentTerm) {
#ifdef RAFT_SHOW
		writeSaid("Refuse(1) to append Node_" + std::to_string(appendEntries.leaderId));
#endif // RAFT_SHOW
		appendEntriesReply.success = false;
		appendEntriesReply.term = currentTerm;
		//return is_change_role;
	}
	else if (logs.getTerm(appendEntries.prevLogIndex) != appendEntries.prevLogTerm) {
#ifdef RAFT_SHOW
		writeSaid("Refuse(2) to append Node_" + std::to_string(appendEntries.leaderId));
		writeSaid("getTerm: " + std::to_string(logs.getTerm(appendEntries.prevLogIndex)));
		writeSaid("leader prevLogTerm: " + std::to_string(appendEntries.prevLogTerm));
		writeSaid("leader prevLogIndex[ " + std::to_string(appendEntries.prevLogIndex));
#endif // RAFT_SHOW
		appendEntriesReply.success = false;
		appendEntriesReply.term = currentTerm;
		//return is_change_role;
	}                          //log????????????0
	// agreement
	else {
		// update logs
		logs.deleteLogs(appendEntries.prevLogIndex + 1);	//appendEntries.prevLogIndex
		for (auto one_log : appendEntries.entries) {
			logs.addLog(one_log);
		}


		if (appendEntries.leaderCommit > commitIndex) {
			commitIndex = appendEntries.leaderCommit;             //??????????????????????
#ifdef RAFT_SHOW
			writeSaid("Update commitIndex[" + std::to_string(commitIndex) + "]");
#endif // RAFT_SHOW
		}

		if (commitIndex > lastApplied) {
			//apply(logs[lastApplied - commitIndex]); //??????????,????????????

			//std::cout << "apply" << std::endl;
			//answerExaminer();
			//writeSaid("answer successfully");

			applyAppendEntries();
			if (idRead_guard() != appendEntries.leaderId) //????leaderId
					idWrite_guard(appendEntries.leaderId);
			lastApplied = commitIndex;
		}

#ifdef RAFT_SHOW
		writeSaid("Agree to append Node_" + std::to_string(appendEntries.leaderId));
#endif // RAFT_SHOW

		appendEntriesReply.success = true;
		appendEntriesReply.term = currentTerm;
		raftTimer.Reset(FOLLOWER);		// add reset

	}

	answer::AdditionAnswer	ans(appendEntriesReply.term,	
								appendEntriesReply.success,
								appendEntriesReply.nodeId
								);						// in order!!!
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

	// discard the remaining VoteAnswer
	discardVoteAnswer();	// not necessary

	//// new way to get answer avoid bug
	//std::shared_ptr<answer::Answer> req = nullptr;
	//if (com.hasAnswer(answer::Kind::AdditionAnswer)) {
	//	req = com.getAnswer(answer::Kind::AdditionAnswer);
	//}
	//else return false;
		
	auto req = com.getAnswer(answer::Kind::AdditionAnswer);
	if (req == nullptr)  return false;
		
	appendEntriesReply.nodeId = req->getReceiverId();
	appendEntriesReply.success = req->getAttitude();
	appendEntriesReply.term = req->getReceiverTerm();


#ifdef RAFT_DEBUG
	writeSaid("Get apppend answer. success = " + std::to_string(appendEntriesReply.success));
#endif // RAFT_DEBUG

	// agreement
	if (appendEntriesReply.success) {	// modify
		nextIndex[appendEntriesReply.nodeId] = logs.getNewest() + 1;
		if (matchIndex[appendEntriesReply.nodeId] != logs.getNewest()) {
			matchIndex[appendEntriesReply.nodeId] = logs.getNewest();
#ifdef RAFT_SHOW
			writeSaid("Got agreement, matchIndex[" + std::to_string(appendEntriesReply.nodeId)
				+ "] change to " + std::to_string(matchIndex[appendEntriesReply.nodeId]));
#endif // RAFT_SHOW
			//matchTotal++;
			checkMatchIndex();
		}
	}
	// reject
	else {
		if (currentTerm < appendEntriesReply.term) {
#ifdef RAFT_SHOW
			writeSaid("Term problem, from Leader to Follower");
#endif // RAFT_SHOW
			setTerm(appendEntriesReply.term);		// use setTerm
			is_change_role = true;
			changeRole(FOLLOWER);
			//return is_change_role;
		}
		else if (nextIndex[appendEntriesReply.nodeId] > 1) {
			nextIndex[appendEntriesReply.nodeId]--;
#ifdef RAFT_DEBUG
			writeSaid("nextIndex[" + std::to_string(appendEntriesReply.nodeId) 
				+ "] change to " + std::to_string(nextIndex[appendEntriesReply.nodeId]));
#endif // RAFT_DEBUG
		}
	}

	return is_change_role;
}


void Raft::checkMatchIndex()
{
	// check every log that is not commited
	IndexType last_commit_index = commitIndex;
	for (IndexType check_index = logs.getNewest();
		check_index > last_commit_index; --check_index) {
		int this_match_total = 1;	// matchTotal for this log, myself is match
		for (auto f_id : nodeIdList) {
			if (f_id == nodeId) // not check myself 
				continue;
			if (matchIndex[f_id] >= check_index)
				++this_match_total;
		}
		if (this_match_total > nodeTotal / 2) {
			commitIndex = check_index; 
#ifdef RAFT_SHOW
			writeSaid("Update commitIndex[" + std::to_string(commitIndex) + "]");
#endif  //RAFT_SHOW
			// apply(log[lastapplied - commitIndex]);        //????????????????????
			//std::cout << "apply" << std::endl;

			//writeSaid("Write into account???");//

			applyAppendEntries();
			
			answerExaminer();
			
#ifdef RAFT_DEBUG
			
#endif // RAFT_DEBUG

			
			// discard the remaining AdditionAnswer
			//discardAdditionAnswer();
			lastApplied = commitIndex;
			break;	// If the back matches, the front doesn't need to be checked
		}
	}

	
}
void Raft::applyAppendEntries() {

	std::vector<LogType> applyLog;
	logs.getLog(lastApplied+1, commitIndex, applyLog);
	
	writeSaid("ready to apply");
	
	/*
	std::vector<LogType>::iterator i;
	for (i = applyLog.begin(); i != applyLog.end(); i++) {
		std::cout << *i << std::endl;
		writeSaid(*i);
		//client.Write_account(*i);
	}*/
	int i;
	for (i = 0; i < applyLog.size(); i++) {
		std::cout << applyLog[i] << std::endl;
		writeSaid("Write into account");
		writeSaid(applyLog[i]);

		if (client.Write_account(applyLog[i]))
			writeSaid("successfully applied");
		else
			writeSaid("apply failed");
	}

}