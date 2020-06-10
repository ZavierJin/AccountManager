#pragma once
#include<mutex>
#include"raft_type.h"
#include"client.h"

typedef enum GlobalState {
	INITIAL,
	CHECK_TO_RAFT,
	RAFT_GET,
	RAFT_TO_CHECK,
	CHECK_GET
}GlobalStateType;
typedef int GlobalIdType;

extern std::mutex mtx;
extern std::string action;//�����˷����ַ���
extern GlobalStateType Global_State;//��raft��string��״̬����,state=1��ʾ��raft�����ַ���

//extern int  leaderId; //������һ�䵼�¶����ظ�������Ϊidtype������˴���Ϊint ����
extern GlobalIdType Global_leaderId;
//extern auto& com = computer::Computer::instance();

extern Client client;

extern int Global_nodeId;

GlobalStateType stateRead_guard(void);
void stateWrite_guard(GlobalStateType i);
std::string actionRead_guard(void);//��action����д
void actionWrite_guard(std::string s);

GlobalIdType idRead_guard(void);
void idWrite_guard(GlobalIdType id);
