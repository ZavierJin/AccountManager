/* Timer.h
** Brief introduction: definition of Raft related Timer class
** Create by robotics 2020/5/28
** Modify:
**      2020/6/3 (by Jin Zhanyu):
**          #include "raft_type.h"
*/

#ifndef RAFT_TIMER_H
#define RAFT_TIMER_H

#include <ctime>
#include "raft_type.h"

class Timer {

public:
    explicit Timer(StateType initState);
    void Reset(StateType changeState);
    bool Timeout();
    clock_t getNowTime();
    ~Timer();

private:
    StateType nodeState;
    clock_t startTime;
    clock_t timePeriod;
    void getPeriod();
};


#endif //RAFT_TIMER_H
