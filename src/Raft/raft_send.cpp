/* Raft.cpp
** Brief introduction: the implementation of Raft's method about sending messages
** Create by Jin Zhanyu 2020/5/29
*/

#include "Raft.h"

void Raft::sendVote()
{
    if(nodeState != CANDIDATE) return;

	auto& com = computer::Computer::instance();
	TermType lastLogTerm = 0;// logs.getTerm(commitIndex);
    
    for(auto rec_id : nodeIdList){ // Get each nodeId from nodeIdList 
        if(rec_id == nodeId) // not send to myself 	
            continue;
		request::Vote req(currentTerm, nodeId, commitIndex, lastLogTerm);
		com.sendRequest(req, rec_id);
#ifdef SHOW
		f_out << "Send vote to " << "Node_" << rec_id << std::endl;
#endif // SHOW
    }
}

void Raft::sendAppendEntries() 
{
    if(nodeState != LEADER) return;

	//auto& com = computer::Computer::instance();
	AppendEntries	addition_msg;		// AppendEntries message

	addition_msg.term = currentTerm;
	addition_msg.leaderId = nodeId;
	addition_msg.leaderCommit = commitIndex;
	// IndexType lastMatch;      // Log index to which this node was last synchronized

    for(auto rec_id : nodeIdList){ // Get each nodeId from nodeIdList 
        if(rec_id == nodeId) // not send to myself 
            continue;
        // Some of the message content sent to each node is different
		addition_msg.prevLogIndex = nextIndex[rec_id] - 1;
		addition_msg.prevLogTerm = 1;// logs.getTerm(addition_msg.prevLogIndex);
        // lastMatch = matchIndex[rec_id];
		// addition_msg.entries.copyLogs(logs, lastMatch);	// incorrect 
        // Send message
		/*
		request::Addition req(
			addition_msg.term,
			addition_msg.leaderId,
			addition_msg.prevLogIndex,
			addition_msg.prevLogTerm,
			addition_msg.leaderCommit,
			logs.getBegin(nextIndex[rec_id]),		 
			logs.getEnd()			
		);		
		com.sendRequest(req, rec_id);*/
		f_out << "Send append entries to " << "Node_" << rec_id << std::endl;
    }
}

void Raft::setTerm(TermType new_term)
{
    currentTerm = new_term;
    votedFor = INVALID_ID;      // reset for new term
}


//void Raft::answerVote() 
//{
//	answer::VoteAnswer	ans(currentTerm, voteGranted);
//	com.sendAnswer(ans, /* receiver's Address*/);
//}
//
//void Raft::answerAppendEntries()	
//{
//	answer::AdditionAnswer	ans(currentTerm, success);
//	com.sendAnswer(ans, /* receiver's Address*/);
//}



