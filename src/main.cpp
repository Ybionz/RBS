#include <iostream>
#include <vector>
#include <list>
#include <random> // for std::mt19937
#include <ctime>  // for std::time
#include <set>    // for std::unordered_set

#include "node.h"
#include "map.h"
#include "state.h"
#include "aStar.h"

// std::set<int, decltype([](const auto &a, const auto &b) {
//      return /* compare a and b */;
// })> s;

// bool comparePtrToNode(State *a, State *b) { return (*a < *b); }

// std::list<Action> AStar(Node *start, Node *end, Map *m)
// {
//     State initial{start, end, m};
//     auto visited_compare{[](State *a, State *b) {
//         if (a->currentNode < b->currentNode)
//         {
//             return true;
//         }
//         else if (a->currentNode > b->currentNode)
//         {
//             return false;
//         }
//         if (a->action.dir < b->action.dir)
//         {
//             return true;
//         }
//         return false;
//     }};
//     std::set<State *, decltype(visited_compare)> visited;
//     std::set<State *, decltype([](State *a, State *b) { return (*a < *b); })> open;
//     std::set<State *, decltype([](State *a, State *b) { return (*a < *b); })> closed;
//     open.insert(&initial);
//     while (!open.empty())
//     {

//         State *nextState = open.extract(open.begin()).value();
//         closed.insert(nextState);

//         if (nextState->atGoal())
//         {
//             return nextState->getPath();
//         };
//         for (State *s : nextState->getChildStates())
//         {
//             open.insert(s);
//         };
//     };
//     std::list<Action> failed{};
//     return failed;
// };

int main()
{
    int n{100};
    // Map m(1, 2, 0.0);
    Map m(n, n, 0.2);
    m.setFreeSpace(n - 1, n - 1);

    // m.setWall(0, 1);
    AStar aStar{AStar(m.getNode(0, 0), m.getNode(n - 1, n - 1), &m)};
    // AStar aStar{AStar(m.getNode(0, 0), m.getNode(0, 1), &m)};
    std::list<Action> path{aStar.search()};

    if (path.size() > 0)
    {
        m.printMap(*m.getNode(0, 0), path);
    }
    else
    {
        m.printMap();
    }
    // for (auto a : path)
    // {
    //     std::cout << a << '\n';
    // };

    return 0;
}