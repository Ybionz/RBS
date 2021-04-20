#include "aStar.h"

AStar::AStar(Node *startNode, Node *goalNode, Map *m)
    : start{startNode},
      goal{goalNode},
      map{m},
      visitedCompare(getVisitedCompare()){};

std::function<bool(State *, State *)> AStar::getVisitedCompare()
{
    auto visitedCompare{[](State *a, State *b) {
        if (a->currentNode < b->currentNode)
        {
            return true;
        }
        else if (a->currentNode > b->currentNode)
        {
            return false;
        }
        if (a->action.dir < b->action.dir)
        {
            return true;
        }
        return false;
    }};
    return visitedCompare;
};

std::list<Action> AStar::search()
{
    State initial{start, goal, map};
    auto visitedCompare{[](State *a, State *b) {
        if (*a->currentNode < *b->currentNode)
        {
            return true;
        }
        else if (*a->currentNode > *b->currentNode)
        {
            return false;
        }
        if (a->action.dir < b->action.dir)
        {
            return true;
        }
        return false;
    }};
    std::set<State *, decltype(visitedCompare)> visited;
    std::set<State *, decltype([](State *a, State *b) { return (*a < *b); })> open;
    std::set<State *, decltype([](State *a, State *b) { return (*a < *b); })> closed;
    open.insert(&initial);
    visited.insert(&initial);
    while (!open.empty())
    {

        State *nextState = open.extract(open.begin()).value();
        closed.insert(nextState);

        if (nextState->atGoal())
        {
            return nextState->getPath();
        };
        for (State *s : nextState->getChildStates())
        {
            if (visited.find(s)==visited.end())
            {
                visited.insert(s);
                open.insert(s);
            }
        };
    };
    std::list<Action> failed{};
    return failed;
};
