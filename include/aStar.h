#ifndef ASTAR_H
#define ASTAR_H

#include <set>
#include <list>
#include <functional>

#include "action.h"
#include "map.h"
#include "node.h"
#include "state.h"
#include "actionConstraint.h"

class AStar
{
public:
    AStar(Node *startNode, Node *goalNode, Map *m, std::set<ActionConstraint> constraints);
    std::list<Action> search();
    std::function<bool(State *, State *)> getVisitedCompare();

    Node *start;
    Node *goal;
    Map *map;
    std::function<bool(State *, State *)> visitedCompare;
    std::set<ActionConstraint> constraints;
    // std::map<
};

#endif