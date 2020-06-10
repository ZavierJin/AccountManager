#include"Global.h"


std::mutex mtx;
std::string action;//�����˷����ַ���
GlobalStateType Global_State=INITIAL;//��raft��string��״̬����,state=1��ʾ��raft�����ַ���
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

