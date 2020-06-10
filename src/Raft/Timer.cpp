//
// Created by robotics on 2020/5/28.
//

#include "Timer.h"
#include <ctime>
#include <cstdlib>
#include <Windows.h>

#define minTime 7000		// modify
#define maxTime 10000
#define BeatInterval 500

#ifdef RANDOM_SLEEP
	#define SleepTime 15000
	#define SleepInterval 20000
	bool had_sleep = false;
#endif // RANDOM_SLEEP



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

#ifdef RANDOM_SLEEP
bool Timer::randomSleep() const
{
	bool need_to_change_role = false;
	if (!had_sleep && (clock() - sleep_count_start) > SleepTime) {
		had_sleep = true;
		std::cout << "Start sleep." << std::endl;
		Sleep(SleepInterval);
		std::cout << "Start over!!" << std::endl;
		need_to_change_role = true;
	}
	return need_to_change_role;
}
#endif // RANDOM_SLEEP
