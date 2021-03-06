/* raft_thread.cpp
** Brief introduction: raft thread
** Create by Jin Zhanyu 2020/6/3
*/
#include "Raft.h"
#include<iostream>
void raft_test()
{
#ifdef JPDEBUG
	std::cout << "Thread raft_test starts" << std::endl;
#endif // JPDEBUG


	Raft raft;
#ifdef JPDEBUG
	std::cout << "Raft is initailizing" << std::endl;
#endif // JPDEBUG
	


	while (true)
	{
		StateType now_state = raft.getState();
		bool is_change_role = false;

		switch (now_state)
		{
		case FOLLOWER:	
			is_change_role = raft.acceptAppendEntries();
			if (is_change_role)	break;

			is_change_role = raft.electionTimeOut();
			if (is_change_role)	break;

			is_change_role = raft.acceptVote();
			if (is_change_role)	break;

			break;

		case CANDIDATE:	
			is_change_role = raft.acceptAppendEntries();
			if (is_change_role)	break;

			is_change_role = raft.electionTimeOut();
			if (is_change_role)	break;

			is_change_role = raft.acceptVote_request();
			if (is_change_role)	break;

			is_change_role = raft.acceptVote();
			if (is_change_role)	break;

			break;

		case LEADER:
			raft.beatTimeOut();
			is_change_role = raft.acceptAppendEntriesReply();
			if (is_change_role)	break;

			is_change_role = raft.acceptAppendEntries();
			if (is_change_role)	break;

			is_change_role = raft.acceptVote();
			if (is_change_role)	break;

			raft.receiveExaminer();	// try to get request from Examiner


		
			//raft.answerExaminer();
		

			break;

		default:
			break;
		}
	}
}