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
#include <fstream>
#include "Computer.h"

typedef enum NodeState {
	FOLLOWER, CANDIDATE, LEADER
}StateType;

typedef int TermType;
typedef int IdType;
typedef int IndexType;
typedef std::string LogType;

#define NODE_TOTAL 7			// node total
#define INITIAL_TERM 0			// initial term
#define INITIAL_INDEX 0			// initial index, 0 or 1?
#define INITIAL_STATE FOLLOWER	// initial state
#define INVALID_ID -1			// invalid id 

#define RAFT_SHOW	1
#define RAFT_DEBUG	1
//#define RANDOM_SLEEP 1
 

