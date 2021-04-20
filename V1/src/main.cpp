#include <iostream>
#include <vector>
#include <list>
#include <random> // for std::mt19937
#include <ctime>  // for std::time
#include <set>    // for std::unordered_set

#include "node.h"
#include "map.h"
#include "state.h"

// std::set<int, decltype([](const auto &a, const auto &b) {
//      return /* compare a and b */;
// })> s;

// bool comparePtrToNode(State *a, State *b) { return (*a < *b); }

std::list<Action> AStar(Node* start, Node* end, Map *m)
{
    State initial{start, end, m};
    std::set<State *, decltype([](State *a, State *b) { return (*a < *b); })> open;
    std::set<State *, decltype([](State *a, State *b) { return (*a < *b); })> closed;
    open.insert(&initial);
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
            open.insert(s);
        };
    };
    std::list<Action> failed{};
    return failed;
};

int main()
{
    Map m(3, 3, 0.0);

    m.setWall(0, 1);
    std::list<Action> path{AStar(m.getNode(0, 0), m.getNode(0, 2), &m)};

    m.printMap();
    for (auto a : path)
    {
        std::cout << a << '\n';
    };

    return 0;
}