#pragma once
#include<mutex>
#include<string>
extern std::mutex mtx;
extern std::string action;//存放审核发送字符串
extern int state ;//向raft传string的状态变量,state=1表示向raft传递字符串
void test_check();
void check();

typedef enum GlobalState {
	INITIAL, 
	CHECK_TO_RAFT,
	RAFT_GET,
	RAFT_TO_CHECK,
	CHECK_GET
}GlobalStateType;

GlobalStateType stateRead_guard(void);
void stateWrite_guard(GlobalStateType i );
std::string actionRead_guard(void);//对action加锁写
void actionWrite_guard(std::string s);

