/* Raft.h
** Brief introduction: definition of Raft class
** Create by Jin Zhanyu 2020/5/18
** Modify:
**      2020/5/29 (by Jin Zhanyu):
**          1. Split log and term, store shared_ptr<Request> in log 
**          2. integrate answerVote & answerAppendEntries into accept fuctions 
**          3. delete global message variable
**          4. change nextIndex & matchIndex from array to map
**      2020/5/31 (by Jin Zhanyu):
**          1. add  setTerm() 
**          2. separate class Timer & class Logs into files
**          3. add 	LogType  &  INVALID_ID
**      2020/6/3 (by Jin Zhanyu):
**          1. add  matchTotal
**          1. remove some const
*/

#ifndef RAFT_H
#define RAFT_H

#include "Timer.h"
#include "Logs.h"
#include "raft_type.h"


// Message type
struct AppendEntries {
	TermType    term;
	IdType      leaderId;
	IndexType   prevLogIndex;
	TermType    prevLogTerm;
	IndexType   leaderCommit;
	Logs        entries;
};

struct AppendEntriesReply {
	IdType      nodeId;
	TermType    term;
	bool        success;
};

struct Vote {
	TermType    term;
	IdType      candidateId;
	IndexType   lastLogIndex;
	TermType    lastLogTerm;
};


class Raft
{
public:
	Raft();

	void acceptAppendEntries();	// 3
	void acceptAppendEntriesReply(); // leader
	bool acceptVote(); // 3
	bool acceptVote_request();	// candidate

	void sendVote();              // only for candidate
	void sendAppendEntries();     // only for leader
		
	// Interaction with Examiner
	void receiveExaminer();				
	void answerExaminer() const;     
		
	// change state or term
	void changeRole(StateType nowState);
	void setTerm(TermType new_term);
	void resetLeaderPara();

	void showMyInfo();
	void writeSaid(const std::string& raft_said);

	// get state
	bool electionTimeOut();
	StateType getState() const { return nodeState; }
private:
	IdType      nodeId = INVALID_ID;		// random number, not increased from 0
	IdType      leaderId = INVALID_ID;
	StateType   nodeState = INITIAL_STATE;
	int         nodeTotal = NODE_TOTAL;
	int         votedTotal = 0;
	int			matchTotal = 0;
	TermType    currentTerm = INITIAL_TERM;
	IdType      votedFor = INVALID_ID;
	IndexType   commitIndex = INITIAL_INDEX;
	IndexType   lastApplied = INITIAL_INDEX;

	//Logs        logs;		// how to initialize??
	Timer       raftTimer;

	// Contain all servers' ID
	std::vector<IdType> nodeIdList;		

	// only for leader
	std::map<IdType, IndexType> nextIndex;
	std::map<IdType, IndexType> matchIndex;

	std::ofstream fileWriter;
};

#endif
