#ifndef TYPEALIASES_H
#define TYPEALIASES_H

#include <set>
#include <map>
#include <list>

// #include "actionConstraint.h"
// #include "node.h"
// #include "state.h"
class State;
class ActionConstraint;
class Node;

using ACSet_t = std::set<ActionConstraint>;
using agentID_t = int;
using constraints_t = std::map<agentID_t, ACSet_t>;
using constraintSet_t = std::set<constraints_t>;
using task_t = std::pair<Node *, Node *>;
using tasks_t = std::map<agentID_t, task_t>;
using path_t = std::list<const State *>;
using paths_t = std::map<agentID_t, path_t>;

#endif