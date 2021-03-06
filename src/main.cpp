#include <iostream>
#include <vector>
#include <list>
#include <random>    // for std::mt19937
#include <ctime>     // for std::time
#include <set>       // for std::unordered_set
#include <algorithm> // for std::max
// #include <sys/time.h>
// #include <sys/resource.h>
#include "typeAliases.h"

#include "action.h"
#include "aStar.h"
#include "cbs.h"
#include "HLState.h"
#include "map.h"
#include "node.h"
#include "ruleRequest.h"
#include "ruleRestrict.h"
#include "state.h"

struct vertexCompare
{
    bool operator()(const State *a, const State *b) const
    {
        return (*a->currentNode < *b->currentNode);
    };
};

subtasks_t getHelp(path_t path, RuleRequest rule)
{
    subtasks_t tasks{};
    for (auto s : path)
    {
        LightState ls{s->getLS()};
        auto help = rule.getRequest(ls);
        // if(help.size()<1)
        // tasks.push_back();
    }
    return tasks;
}

int main()
{
    int mapSize{10};
    int numAgents{5};
    double objects{0.0};
    Map m(mapSize, mapSize, objects, numAgents);

    std::set<agentID_t> agents;
    for (agentID_t agent{0}; agent < numAgents; agent++)
    {
        agents.insert(agent);
    };
    RuleRestrict ruleRS(agents, &m);
    ruleRS.makeDistance(1);
    std::set<RuleRestrict> rulesRS{ruleRS};
    m.setRules(rulesRS);
    CBS cbs{&m, agents, rulesRS};
    paths_t paths = cbs.search();
    m.saveLevel("open", paths);

    // subtask_t sub1{Move(Action::Direction::south, m.getNode(0, 2))};

    // subtasks_t subtasks{sub1};
    // if (subtasks[0].contains(Move(Action::Direction::south, m.getNode(0, 2))))
    //     std::cout << "yes\n";
    // else
    //     std::cout << "no:(\n";

    // auto astar{AStar(m.getNode(0, 0), m.getNode(3, 0), 0, &m, ACSet_t{}, subtasks)};

    // auto path = astar.search();
    // paths_t pathstask{{0, path}};
    // mission_t mis{m.getNode(0, 0), m.getNode(3, 0)};
    // m.setMissions(missions_t{{0, mis}});
    // m.saveLevel("tasktest", pathstask);

    // RuleRequest ruleRQ(std::set<agentID_t>{0}, std::set<agentID_t>{1}, &m);
    // ruleRQ.makeSokoban();

    // std::set<RuleRequest> rulesRQ{ruleRQ};
    // m.setRules(rulesRS, rulesRQ);

    // auto pathsTemp = AStar(m.getNode(1, 1), m.getNode(1, 3), 0, &m, ACSet_t{}, subtasks_t{}, &rulesRQ).search();
    // auto help = getHelp(pathsTemp, ruleRQ);

    // auto pathHelper = AStar(m.getNode(0, 0), m.getNode(4, 0), 1, &m, ACSet_t{}, help).search();

    // rlimit rlim{};
    // auto out = getrlimit(0, &rlim);

    // {
    //     int n{100000000};
    //     std::set<int> states;
    //     for (int i{0}; i < n; i++)
    //     {
    //         states.insert(i);
    //     }
    //     std::cout << sizeof(states) << '\n';

    //     states.clear();
    // }
    // State *test = new State(m.getNode(0, 0), m.getNode(0, 0), 0, &m);
    // // int *test = new int{10};
    // std::cout << sizeof(test) << '\n';
    // std::cout << sizeof(*test) << '\n';

    // delete test;
    // std::cout << sizeof(test) << '\n';
    // std::cout << sizeof(*test) << '\n';
    // std::cout << "currentNode " << *test->currentNode << '\n';

    // std::cout << sizeof(HLState) << '\n';

    // for (int i{0}; i < n; i++)
    // {
    //     states.insert(new State(m.getNode(0, 0), m.getNode(0, 0), 0, &m));
    // }

    // std::for_each(states.begin(), states.end(), [](State *state) { delete state; });
    // states.clear();

    // CBS cbs{&m, agents, rulesRS, rulesRQ};

    // tasks_t tasks = m.getTasks(numAgents);

    // for (auto [agent, task] : tasks)
    // {
    //     std::cout << "Agent " << agent << " has task " << *task.first << " to " << *task.second << '\n';
    // }

    // HLState *initial = new HLState(tasks, &m);

    // std::multiset<HLState *, decltype([](HLState *a, HLState *b) { return (*a < *b); })> open;
    // std::multiset<HLState *, decltype([](HLState *a, HLState *b) { return (*a < *b); })> closed;

    // open.insert(initial);

    // HLState *nextState;
    // paths_t paths;
    // while (!open.empty())
    // {
    //     nextState = open.extract(open.begin()).value();
    //     closed.insert(nextState);

    //     if (nextState->isGoal())
    //     {
    //         paths = nextState->getPaths();
    //         break;
    //         // return temp;
    //     };

    //     for (HLState *s : nextState->getChildStates())
    //     {
    //         // if (visited.find(s) == visited.end())
    //         if (open.find(s) == open.end() && closed.find(s) == closed.end())
    //         {
    //             // visited.insert(s);
    //             open.insert(s);
    //         }
    //     };
    // }
    // std::cout << "Depth of goal is " << nextState->getG() << '\n';
    // for (auto [agent, path] : paths)
    // {
    //     std::cout << "Agent " << agent << " has path: \n";
    //     for (auto state : path)
    //     {
    //         std::cout << *state->currentNode << state->action << '\n';
    //     }
    // }

    // std::cout << "Size of paths is " << paths.size() << '\n';
    // auto tasks{m.getValidTasks(2)};
    // for (int i{0}; i < 2; ++i)
    // {
    //     auto task = tasks[i];
    //     std::cout << "Agent " << i << " has start at " << *task.first << " and goal at " << *task.second << '\n';
    //     paths[i] = AStar(task.first, task.second, i, &m, std::set<ActionConstraint>{}).search();
    // }

    // std::map<int, std::pair<Node *, Node *>> tasks{{0, {m.getNode(0, 1), m.getNode(2, 1)}}, {1, {m.getNode(1, 0), m.getNode(1, 2)}}};
    // auto tempPath{AStar(tasks[0].first, tasks[0].second, 0, &m, std::set<ActionConstraint>{}).search()};
    // paths[0] = AStar(tasks[0].first, tasks[0].second, 0, &m, std::set<ActionConstraint>{}).search();
    // paths[1] = AStar(tasks[1].first, tasks[1].second, 1, &m, std::set<ActionConstraint>{}).search();
    // auto constraints{getConstraints(paths)};
    // int i{0};
    // std::map<agentID_t, paths_t> childPaths;
    // for (auto child : constraints)
    // {
    //     std::cout << "Child " << i << " has the constraints:\n";
    //     for (int agent{0}; agent < 2; ++agent)
    //     {
    //         if (child.count(agent) < 1)
    //         {
    //             childPaths[i][agent] = paths[agent];
    //             continue;
    //         }
    //         auto constraint = child[agent];
    //         std::cout << "Agent " << agent << " has constraints \n";
    //         for (auto aCon : constraint)
    //         {
    //             std::cout << aCon << '\n';
    //         }
    //         childPaths[i][agent] = AStar(tasks[agent].first, tasks[agent].second, agent, &m, constraint).search();
    //     }

    //     i++;
    // }
    // for (auto [agent, path] : paths)
    // {
    //     std::cout << "Agent " << agent << " has path:\n";
    //     auto test = *path.end();
    //     Node initialNode = *path.front()->currentNode;
    //     std::list<Action> actions;
    //     for (auto state : path)
    //     {
    //         std::cout << "State has current node " << *state->currentNode << " ";
    //         std::cout << state->action << '\n';
    //         actions.push_front(state->action);
    //     }
    //     std::cout << "initial node is " << initialNode << '\n';
    //     const State *testNode{path.back()};
    //     Action testAction = testNode->action;

    //     m.printMap(initialNode, actions);
    // }

    // for (auto childPath : childPaths)
    // {
    //     std::cout << "Child state " << childPath.first << " has paths:\n";
    //     for (auto [agent, path] : childPath.second)
    //     {
    //         std::cout << "Agent " << agent << " has path:\n";
    //         auto test = *path.end();
    //         Node initialNode = *path.front()->currentNode;
    //         std::list<Action> actions;
    //         for (auto state : path)
    //         {
    //             std::cout << "State has current node " << *state->currentNode << " ";
    //             std::cout << state->action << '\n';
    //             actions.push_front(state->action);
    //         }
    //         std::cout << "initial node is " << initialNode << '\n';
    //         const State *testNode{path.back()};
    //         Action testAction = testNode->action;

    //         m.printMap(initialNode, actions);
    //     }
    // }

    // m.printMap()
    // auto task{m.getValidTask()};
    // // AStar aStar{AStar(m.getNode(0, 0), m.getNode(n - 1, n - 1), &m, std::set<ActionConstraint>{})};
    // // aStar = AStar(task.first, task.second, &m, std::set<ActionConstraint>{});
    // std::list<const State *> path{AStar(task.first, task.second, 0, &m, std::set<ActionConstraint>{}).search()};
    // std::cout << "Path of size " << path.size() << '\n';
    // if (path.size() < 1)
    // {
    //     m.printMap();
    //     std::cout << "Start at " << *task.first << " and goal at " << *task.second << '\n';
    //     std::cout << "Start area " << m.areas[m.getNode(task.first)] << " and goal area " << m.areas[task.second] << '\n';
    //     for (Node *node : m.nodes)
    //     {
    //         std::cout << *node << " has the neightbours: ";
    //         for (auto [a, neigh] : m.getNeighbours(node))
    //         {
    //             std::cout << *neigh << ", ";
    //         }
    //         std::cout << '\n';
    //     }
    //     break;
    // }

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