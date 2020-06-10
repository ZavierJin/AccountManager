/* Timer.h
** Brief introduction: definition of Raft related Timer class
** Create by robotics 2020/5/28
** Modify:
**      2020/6/3 (by Jin Zhanyu):
**          #include "raft_type.h"
**		2020/6/4 (by Jin Zhanyu):
**          1. add  startCountDown & getTrueTime
**			2. add	initialTime
**      2020/6/10 (by Jin Zhanyu):
**          1. add  randomSleep & startSleepCount
**          2. add  sleep_count_start
*/

#ifndef RAFT_TIMER_H
#define RAFT_TIMER_H

#include <ctime>
#include "raft_type.h"

class Timer {

public:
    explicit Timer(StateType initState);
    void Reset(StateType changeState);
	void startCountDown();		// add 
    bool Timeout();
    clock_t getNowTime();
	clock_t getTrueTime() const { return clock() - initialTime; }	// add
    ~Timer();
#ifdef RANDOM_SLEEP
	void startSleepCount() { sleep_count_start = clock(); }
	bool randomSleep() const;	// return need_to_change_role
#endif // RANDOM_SLEEP
	
private:
    StateType nodeState;
	clock_t initialTime;	// add, for getTrueTime
    clock_t startTime;
    clock_t timePeriod;
    void getPeriod();
#ifdef RANDOM_SLEEP
	clock_t sleep_count_start;
#endif // RANDOM_SLEEP
};


#endif //RAFT_TIMER_H
