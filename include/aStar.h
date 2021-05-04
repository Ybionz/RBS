#ifndef ASTAR_H
#define ASTAR_H

#include <set>
#include <list>
#include <functional>

#include "typeAliases.h"

#include "action.h"
#include "map.h"
#include "node.h"
#include "state.h"
#include "actionConstraint.h"

class AStar
{
public:
    AStar(Node *startNode, Node *goalNode, agentID_t agent, Map *m, ACSet_t constraints = ACSet_t{});
    path_t search();
    std::function<bool(State *, State *)> getVisitedCompare();

    Node *start;
    Node *goal;
    Map *map;
    agentID_t agent;
    std::function<bool(State *, State *)> visitedCompare;
    std::set<ActionConstraint> constraints;
    // std::map<
};

#endif