#ifndef TYPEALIASES_H
#define TYPEALIASES_H

#include <set>
#include <map>
#include <list>
#include <vector>

// #include "actionConstraint.h"
// #include "node.h"
// #include "state.h"
class ActionConstraint;
class LightState;
class Map;
class Move;
class Node;
class RuleRestrict;
class RuleRequest;
class State;

using ACSet_t = std::set<ActionConstraint>;
using agentID_t = int;
using constraints_t = std::map<agentID_t, ACSet_t>;
using constraintSet_t = std::set<constraints_t>;
using mission_t = std::pair<Node *, Node *>;
using missions_t = std::map<agentID_t, mission_t>;
using path_t = std::list<const State *>;
using paths_t = std::map<agentID_t, path_t>;
using subtask_t = std::vector<LightState>;
using subtasks_t = std::vector<subtask_t>;

#endif