/* Logs.h
** Brief introduction: definition of Raft related Logs class
** Created by robotics on 2020/5/28.
** Modify:
**      2020/6/3 (by robotics):
**		2020/6/4 (by Jin Zhanyu):
**          1. add  addAction
**          2. modify  getTerm  &  termTrans
**          3. delete new_term para in function addLog
**          4. modify  Logs
**          4. modify  getNewest, return number -1
*/
#ifndef RAFT_LOGS_H
#define RAFT_LOGS_H

#include "raft_type.h"

class Logs {
public:
	Logs();
	~Logs();
	void addLog(const LogType& new_log); // 末尾添加单条日志
	void deleteLogs(IndexType index);    // index之后都被删除(包括index!!!）
	std::vector<LogType>::iterator getBegin(IndexType index);
	std::vector<LogType>::iterator getEnd();
	IndexType getNewest();			// return index of the last log in logList
	TermType getTerm(IndexType index);

	// 末尾添加审核传输的action, 不commit
	void addAction(const std::string& action, const TermType& new_term);
private:
	std::vector<LogType>	logList;  // Split log and term
	TermType termTrans(LogType _log);
};


#endif //RAFT_LOGS_H
