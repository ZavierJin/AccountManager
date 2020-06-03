/* Timer.h
** Brief introduction: definition of Raft related Logs class
** Create by robotics 2020/5/28
** Modify:
**      2020/6/3 (by Jin Zhanyu):
**          #include "raft_type.h"
*/

#ifndef RAFT_LOGS_H
#define RAFT_LOGS_H

#include <iostream>
#include <vector>
#include <memory>
#include "raft_type.h"

class Logs {
public:
	// 实现还没写好
	Logs();
	void addLog(const LogType& new_log);          // 末尾添加单条日志
	void deleteLogs(IndexType index);              // index之后都被删除
	std::vector<LogType>::iterator getBegin(IndexType index);
	std::vector<LogType>::iterator getEnd();
	IndexType getNewest();
	TermType getTerm(IndexType index);
	~Logs();
private:
	std::vector<LogType>	logList;  // Split log and term

};


#endif //RAFT_LOGS_H

