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
    int n{4};
    Map m(n, n, 0.3);
    // AStar aStar;
    while (true)
    {
        // std::cout << "Map has numAreas = " << m.getNumAreas() << '\n';
        m.newMap();
        auto task{m.getValidTask()};
        // AStar aStar{AStar(m.getNode(0, 0), m.getNode(n - 1, n - 1), &m, std::set<ActionConstraint>{})};
        // aStar = AStar(task.first, task.second, &m, std::set<ActionConstraint>{});
        std::list<Action> path{AStar(task.first, task.second, &m, std::set<ActionConstraint>{}).search()};
        std::cout << "Path of size " << path.size() << '\n';
        if (path.size() < 1)
        {
            m.printMap();
            std::cout << "Start at " << *task.first << " and goal at " << *task.second << '\n';
            std::cout << "Start area " << m.areas[m.getNode(task.first)] << " and goal area " << m.areas[task.second] << '\n';
            for (Node *node : m.nodes)
            {
                std::cout << *node << " has the neightbours: ";
                for (auto [a, neigh] : m.getNeighbours(node))
                {
                    std::cout << *neigh << ", ";
                }
                std::cout << '\n';
            }
            break;
        }
    }

    // for (auto p : m.getNeighbours(m.getNode(0, 1)))
    // {
    //     std::cout << "action is " << p.first << '\n';
    //     std::cout << "node is " << *p.second << '\n';
    // }

    // Map m(3, 3, 0.0);
    // // m.setWall(0, 1);
    // ActionConstraint constraint1{Action{Action::Direction::south}, 0, m.getNode(0, 0)};
    // ActionConstraint constraint2{Action{Action::Direction::south}, 1, m.getNode(1, 1)};
    // ActionConstraint constraint3{Action{Action::Direction::south}, 2, m.getNode(1, 1)};

    // AStar aStar{AStar(m.getNode(0, 0), m.getNode(0, 2), &m, std::set<ActionConstraint>{constraint1, constraint2, constraint3})};
    // std::list<Action> path{aStar.search()};

    // if (path.size() > 0)
    // {
    //     m.printMap(*m.getNode(task.first), path);
    // }
    // else
    // {
    //     m.printMap();
    // }
    // for (auto a : path)
    // {
    //     std::cout << a << '\n';
    // };

    return 0;
}