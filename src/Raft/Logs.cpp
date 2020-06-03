//
// Created by robotics on 2020/5/28.
//

#include "Logs.h"


Logs::Logs() {
    LogType nullLog = nullptr;
    TermType nullTerm = 0;
    logList.assign(1,nullLog);
    //termList.assign(1,nullTerm);
}

void Logs::addLog(const LogType& new_log) {
    logList.push_back(new_log);
   // termList.push_back(new_term);
}

void Logs::deleteLogs(IndexType index) {
    logList.erase(logList.begin()+index,logList.end());
    //termList.erase(termList.begin()+index,termList.end());
}

TermType Logs::getTerm(IndexType index) {
	return 0;//termList[index];
}

IndexType Logs::getNewest() {
    return logList.size();
}

Logs::~Logs() {
    logList.clear();
    //termList.clear();
}

std::vector<LogType>::iterator Logs::getBegin(IndexType index) {
    return logList.begin()+index;
}

std::vector<LogType>::iterator Logs::getEnd() {
    return logList.end();
}

