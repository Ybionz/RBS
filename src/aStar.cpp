#include "aStar.h"

AStar::AStar(Node *startNode,
             Node *goalNode,
             int agent,
             Map *m,
             ACSet_t constraints,
             subtasks_t _subtasks,
             std::set<RuleRequest> *_reqRules)
    : start{startNode},
      goal{goalNode},
      map{m},
      agent{agent},
      constraints{constraints},
      subtasks{_subtasks},
      visitedCompare{getVisitedCompare()},
      reqRules{_reqRules} {};

std::function<bool(State *, State *)> AStar::getVisitedCompare()
{
    auto visitedCompare{[](State *a, State *b)
                        {
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

template <typename Set>
void AStar::deleteStates(Set set)
{
    std::for_each(set.begin(), set.end(), [](State *state)
                  { delete state; });
    set.clear();
};

path_t AStar::copyPath(path_t path)
{
    std::list<const State *> temp;
    for (auto state : path)
    {
        temp.push_back(new State{*state});
    };
    return temp;
};

path_t AStar::search()
{
    // auto visitedCompare{[](State *a, State *b) {
    //     if (*a->currentNode < *b->currentNode)
    //     {
    //         return true;
    //     }
    //     else if (*a->currentNode > *b->currentNode)
    //     {
    //         return false;
    //     }
    //     // if (a->g < b->g)
    //     //     return true;
    //     // else if (a->g > b->g)
    //     //     return false;

    //     if (a->action < b->action)
    //     {
    //         return true;
    //     }
    //     return false;
    // }};
    // std::set<State *, decltype(visitedCompare)> visited;
    std::set<State *, decltype([](State *a, State *b)
                               { return (*a < *b); })>
        open;
    std::set<State *, decltype([](State *a, State *b)
                               { return (*a < *b); })>
        closed;

    State *initial = new State{start, goal, agent, map, subtasks};
    open.insert(initial);
    // visited.insert(initial);
    State *nextState;

    while (!open.empty())
    {

        nextState = open.extract(open.begin()).value();
        closed.insert(nextState);

        if (nextState->atGoal(constraints))
        {

            auto path = nextState->getPath();
            auto cPath = copyPath(path);
            deleteStates(open);
            deleteStates(closed);
            return cPath;
            // return temp;
        };

        for (State *s : nextState->getChildStates(constraints,reqRules))
        {
            // if (visited.find(s) == visited.end())
            if (open.find(s) == open.end() && closed.find(s) == closed.end())
            {
                // visited.insert(s);
                open.insert(s);
            }
        };
    };
    deleteStates(open);
    deleteStates(closed);

    std::list<const State *> failed{};
    return failed;
};
