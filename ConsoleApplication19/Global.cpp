#include"Global.h"


std::mutex mtx;
std::string action;//存放审核发送字符串
GlobalStateType Global_State=INITIAL;//向raft传string的状态变量,state=1表示向raft传递字符串
GlobalIdType Global_leaderId=-1;

Client client;
int Global_nodeId = -1;


GlobalStateType stateRead_guard(void) {
	std::lock_guard<std::mutex> lk(mtx);
	return Global_State;
}

void stateWrite_guard(GlobalStateType type) {
	std::lock_guard<std::mutex> lk(mtx);
	Global_State = type;
}

void actionWrite_guard(std::string s) {
	std::lock_guard<std::mutex> lk(mtx);
	action = s;
}

std::string actionRead_guard(void) {
	std::lock_guard<std::mutex> lk(mtx);
	return action;
}

GlobalIdType idRead_guard(void) {
	std::lock_guard<std::mutex> lk(mtx);
	return Global_leaderId;
}

void idWrite_guard(GlobalIdType id) {
	std::lock_guard<std::mutex> lk(mtx);
	Global_leaderId = id;
}

