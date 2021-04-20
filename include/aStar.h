#ifndef ASTAR_H
#define ASTAR_H

#include <set>
#include <list>
#include <functional>

#include "action.h"
#include "map.h"
#include "node.h"
#include "state.h"

class AStar
{
public:
    AStar(Node *startNode, Node *goalNode, Map *m);
    std::list<Action> search();
    std::function < bool(State *, State *)> getVisitedCompare();

    Node *start;
    Node *goal;
    Map *map;
    std::function<bool(State *, State *)> visitedCompare;
};

#endif