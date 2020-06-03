/* Raft.h
** Brief introduction: definition of Raft related type
** Create by Jin Zhanyu 2020/6/3
*/
#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <map>
#include <memory>
#include "Computer.h"

typedef enum NodeState {
	FOLLOWER, CANDIDATE, LEADER
}StateType;

typedef int TermType;
typedef int IdType;
typedef int IndexType;
typedef std::string LogType;

#define NODE_TOTAL 6			// node total
#define INITIAL_TERM 0			// initial term
#define INITIAL_INDEX 0			// initial index, 0 or 1?
#define INITIAL_STATE FOLLOWER	// initial state
#define INVALID_ID -1			// invalid id 

#define SHOW	1
#define DEBUG	1
