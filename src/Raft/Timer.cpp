//
// Created by robotics on 2020/5/28.
//

#include "Timer.h"
#include <ctime>
#include <cstdlib>

#define minTime 5000		// modify
#define maxTime 15000

Timer::Timer(StateType initState) {
    startTime = clock();
    nodeState = initState;
    getPeriod();
}

void Timer::getPeriod() {
    clock_t randomTime;

    srand((int)time(0));
    randomTime = rand()% (maxTime-minTime)+minTime;

    switch (nodeState)
    {
        case LEADER:
            timePeriod = randomTime/10;
            break;
        default:
            timePeriod = randomTime;
            break;
    }
}

clock_t Timer::getNowTime() {
    clock_t nowTime;
    nowTime = clock();
    return nowTime - startTime;
}

void Timer::Reset(StateType changeState) {
    startTime = clock();
    nodeState = changeState;
	getPeriod();				// add
}

bool Timer::Timeout() {
    return getNowTime() > timePeriod;
}

Timer::~Timer() = default;
