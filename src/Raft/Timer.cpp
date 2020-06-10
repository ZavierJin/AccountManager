//
// Created by robotics on 2020/5/28.
//

#include "Timer.h"
#include <ctime>
#include <cstdlib>

#define minTime 7000		// modify
#define maxTime 10000
#define BeatInterval 500

Timer::Timer(StateType initState) {
    //startTime = clock();
	srand((int)time(0));		// move from getPeriod(), just srand once
    nodeState = initState;
    getPeriod();
}

void Timer::startCountDown(){
	startTime = clock();
	initialTime = clock();
}

void Timer::getPeriod() {
    clock_t randomTime;

    randomTime = rand()% (maxTime-minTime)+minTime;

    switch (nodeState)
    {
        case LEADER:
            timePeriod = BeatInterval;
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
