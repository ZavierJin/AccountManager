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
    ////���ݽڵ����ͷ���time_out
    ////�����follower�򷵻�һ����������ں���Χ�ڣ�
    ////�����candidateҲ����һ���������ѡ�����ڣ���
    ////�����һ��leader�򷵻�һ���ϴ�ĳ�����ʹ����һֱ�ܽ�������Ӧֱ���������쵼��Ϊֹ��

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
    //        switch(raft.readCall) {//��txt�ж�ȡ�Ƿ����ܵ����󣬲�������Ӧ�����������ص���Ӧ
    //            case:NEW_LOG//���Է�������������
    //            {
    //                raft.clientRequest();
    //                //��Ӧ���󣬾���ʵ��ȡ���ڽӿں���
    //                
    //            }
    //            case:VOTE//����ĳһ��candidate��ͶƱ����
    //            {
    //                 raft.voteRequest();
    //               //��Ӧ���󣬾���ʵ��ȡ���ڽӿں���
    //                
    //            } 
    //            case:ADDITIONAL_LOG//�����쵼�˽ڵ�ĸ�����־
    //            {
    //                 raft.addlogRequest();
    //                //��Ӧ���󣬾���ʵ��ȡ���ڽӿں���
    //            }
    //            
    //        }    
    //    }
    //    //����ѭ�������
    //    //1.timeout����
    //    //2.�ڲ�״̬�л���һЩת�����ƣ�����ѭ��
    //    //����ѭ������Ҫ������
    //    //1.����start_time
    //    //2.����time_out
    //    //���岻ͬ���Ͷ���ͬһ����RPC����Ӧ��ѭ���ڲ�����ʵ��
    //    //��ʵ��������һ��Ҳ�������ڲ�ʵ�֣�
    //    //�����������������ʱ������
    //    //������ڲ��жϵ�״̬�л�ֻ��Ҫ�ڲ���������ʱ��ĸ��漴��
    //    start_time  = raft_timer.getCurrentT();
    //    time_out = raft_timer.time_out(raft.state);
    //}

}