/* Raft.h
** Brief introduction: definition of Raft class
** Create by Jin Zhanyu 2020/6/3
*/
#include "Raft.h"

void raft_test()
{
#ifdef DEBUG
	//std::cout << "press 1 to start" << std::endl;
	//int start = 0;
	//std::cin >> start;
	//while (!start) continue;
#endif // DEBUG	

	Raft raft;
	int count = 0;	// debug
    //Timer raft_timer;
    //double start_time = raft_timer.getCurrentT();
    //double time_out = raft_timer.time_out(raft.state);
    ////根据节点类型返回time_out
    ////如果是follower则返回一个随机数（在合理范围内）
    ////如果是candidate也返回一个随机数（选举周期？）
    ////如果是一个leader则返回一个较大的常数（使得其一直能进进行响应直到不再是领导人为止）

	while (true)
	{
		StateType now_state = raft.getState();
		bool is_change_role = false;
#ifdef DEBUG
		//f_out << "[ " << count++ << " ]" << std::endl;
#endif // DEBUG

		switch (now_state)
		{
		case FOLLOWER:	
			is_change_role = raft.electionTimeOut();
			if(is_change_role)	break;
				
			is_change_role = raft.acceptVote();
			if (is_change_role)	break;

			// append
			break;
		case CANDIDATE:	// the same?
			is_change_role = raft.electionTimeOut();
			if(is_change_role)	break;
				
			is_change_role = raft.acceptVote();
			if (is_change_role)	break;

			is_change_role = raft.acceptVote_request();
			if (is_change_role)	break;
			// append
			break;
		case LEADER:
#ifdef DEBUG
			std::cout << "I'm a Leader now!!!!" << std::endl;
			while(true)	continue; // hold on
#endif // DEBUG
			break;
		default:
			break;
		}
	}
    //while(true){
    //    while( (raft_timer.getCurrentT()- start_time <time_out)){
    //        switch(raft.readCall) {//从txt中读取是否有受到请求，并根据相应的请求进行相关的响应
    //            case:NEW_LOG//来自服务器的新请求
    //            {
    //                raft.clientRequest();
    //                //响应请求，具体实现取决于接口函数
    //                
    //            }
    //            case:VOTE//来自某一个candidate的投票请求
    //            {
    //                 raft.voteRequest();
    //               //响应请求，具体实现取决于接口函数
    //                
    //            } 
    //            case:ADDITIONAL_LOG//来自领导人节点的附加日志
    //            {
    //                 raft.addlogRequest();
    //                //响应请求，具体实现取决于接口函数
    //            }
    //            
    //        }    
    //    }
    //    //跳出循环的情况
    //    //1.timeout到了
    //    //2.内部状态切换（一些转换机制）跳出循环
    //    //跳出循环后需要做的事
    //    //1.重置start_time
    //    //2.重置time_out
    //    //具体不同类型对于同一样的RPC的响应由循环内部函数实现
    //    //其实跳出的这一步也可以由内部实现？
    //    //如果是正常跳出（超时）不行
    //    //如果是内部判断的状态切换只需要内部进行两个时间的更替即可
    //    start_time  = raft_timer.getCurrentT();
    //    time_out = raft_timer.time_out(raft.state);
    //}

}