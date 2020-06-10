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
extern std::string action;//存放审核发送字符串
extern GlobalStateType Global_State;//向raft传string的状态变量,state=1表示向raft传递字符串

//extern int  leaderId; //由于这一句导致定义重复？定义为idtype会出错，此处改为int 类型
extern GlobalIdType Global_leaderId;
//extern auto& com = computer::Computer::instance();

extern Client client;

extern int Global_nodeId;

GlobalStateType stateRead_guard(void);
void stateWrite_guard(GlobalStateType i);
std::string actionRead_guard(void);//对action加锁写
void actionWrite_guard(std::string s);

GlobalIdType idRead_guard(void);
void idWrite_guard(GlobalIdType id);
