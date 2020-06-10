/* Logs.h
** Brief introduction: definition of Raft related Logs class
** Created by robotics on 2020/5/28.
** Modify:
**      2020/6/3 (by robotics):
**      2020/6/4 (by Jin Zhanyu):
**          1. add  addAction
**          2. modify  getTerm  &  termTrans
**          3. delete new_term para in function addLog
**          4. modify  Logs
**          4. modify  getNewest, return number -1
**		2020/6/5(by Liu Jiangpin):
**			add getLog
**			function:
**			return the log between index1 and index2
*/
#ifndef RAFT_LOGS_H
#define RAFT_LOGS_H

#include "raft_type.h"

class Logs {
public:
    Logs();
    ~Logs();

    // add action from Examiner
    void addAction(const std::string& action, const TermType& new_term);

    // modify logList
    void addLog(const LogType& new_log); 
    void deleteLogs(IndexType index);    // including index will be deleted!!!
	std::vector<LogType> getLog(IndexType indexBegin, IndexType indexEnd, std::vector<LogType>& tempVec);
    std::vector<LogType>::iterator getBegin(IndexType index);
    std::vector<LogType>::iterator getEnd();
    IndexType getNewest();          // return index of the last log in logList
    TermType getTerm(IndexType index);

private:
    std::vector<LogType>    logList;  // Split log and term
    TermType termTrans(LogType _log);
};


#endif //RAFT_LOGS_H
