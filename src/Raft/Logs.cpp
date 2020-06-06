/* Logs.h
** Created by robotics on 2020/5/28.
** Modify:
**      2020/6/3 (by robotics):
*/

#include "Logs.h"

#define Separator "|"


Logs::Logs() {
    LogType nullLog = "empty header|0";
    logList.push_back(nullLog);
}

void Logs::addLog(const LogType& new_log) {
    logList.push_back(new_log);
}

void Logs::deleteLogs(IndexType index) {
    if ( index < static_cast<IndexType>(logList.size()))
        logList.erase(logList.begin() + index, logList.end());
    //logList.clear();
}

TermType Logs::getTerm(IndexType index) {
    if (index <= 0) return 0;   // empty header
    else if (index >= static_cast<IndexType>(logList.size())) return 0; // invalid index
    else return termTrans(logList[index]);
}

IndexType Logs::getNewest() {
    return static_cast<IndexType>(logList.size()) - 1;
}

std::vector<LogType>::iterator Logs::getBegin(IndexType index) {
    return logList.begin() + index;
}

std::vector<LogType>::iterator Logs::getEnd() {
    return logList.end();
}

TermType Logs::termTrans(LogType _log) {
    int pos;
    pos = _log.find(Separator) + 1;         // modify
    //pos = _log.find_first_of(' ') + 1;
    return stol(_log.substr(pos));
}

void Logs::addAction(const std::string& action, const TermType& new_term) 
{
    logList.push_back(action + Separator + std::to_string(new_term));
}

Logs::~Logs() {
    logList.clear();
}
