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
**          2. remove some const
**          3. add	showMyInfo
**          4. #include "raft_type.h"
**      2020/6/4 (by Jin Zhanyu):
**          1. add  beatTimeOut
**          2. add	writeSaid  &  std::ofstream fileWriter;
**          3. modify the type of entries
**          4. add  discardVoteAnswer  &  discardAdditionAnswer
*/

#ifndef RAFT_H
#define RAFT_H

#include "Timer.h"
#include "Logs.h"
#include "raft_type.h"

//#define	JPDEBUG 1

// Message type
struct AppendEntries {
	TermType    term;
	IdType      leaderId;
	IndexType   prevLogIndex;
	TermType    prevLogTerm;
	IndexType   leaderCommit;
	std::vector<LogType>	entries;
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

	StateType getState() const { return nodeState; }

	// check time
	bool electionTimeOut();	// return is_change_role
	bool beatTimeOut();		// return is_time_out

	// manage raft messages, return is_change_role
	bool acceptAppendEntries();			// 3 state	 
	bool acceptAppendEntriesReply();	// leader
	bool acceptVote();					// 3 state	
	bool acceptVote_request();			// candidate
		
	// Interaction with Examiner
	void receiveExaminer();				
	void answerExaminer();  


private:
	// send msg to other node
	void sendVote();              // only for candidate
	void sendAppendEntries();     // only for leader

	// change state or term
	void changeRole(StateType nowState);
	void setTerm(TermType new_term);

	// others
	void resetLeaderPara();
	void showMyInfo();
	void writeSaid(const std::string& raft_said);
	void discardVoteAnswer();
	void discardAdditionAnswer();

	// check, used in acceptAppendEntriesReply()
	void checkMatchIndex();		



	IdType      nodeId = INVALID_ID;		// random number, not increased from 0
	IdType      leaderId = INVALID_ID;
	StateType   nodeState = INITIAL_STATE;
	int         nodeTotal = NODE_TOTAL;
	int         votedTotal = 0;
	//int			matchTotal = 0;		// Changed to local variable
	TermType    currentTerm = INITIAL_TERM;
	IdType      votedFor = INVALID_ID;
	IndexType   commitIndex = INITIAL_INDEX;
	IndexType   lastApplied = INITIAL_INDEX;

	Logs        logs;		
	Timer       raftTimer;

	// Contain all servers' ID
	std::vector<IdType> nodeIdList;		

	// only for leader
	std::map<IdType, IndexType> nextIndex;
	std::map<IdType, IndexType> matchIndex;

	// record node output
	std::ofstream fileWriter;		

	
};

#endif
