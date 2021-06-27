#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <algorithm>  // for std::max
#include <vector>     // for std::vector
#include <random>     // for std::mt19937
#include <ctime>      // for std::time
#include <list>       // for std::list
#include <map>        // for std::map
#include <set>        // for std::set
#include <cmath>      // for std::sqrt
#include <utility>    // for std::pair
#include <fstream>    // for std::ofstream
#include <filesystem> // for std::filesystem
#include <string>     // for std::string

namespace fs = std::filesystem;

#include "typeAliases.h"
#include "node.h"
#include "action.h"
#include "state.h"
#include "ruleRestrict.h"
#include "ruleRequest.h"
#include "move.h"

class Map
{
    struct Sorter
    {
        bool operator()(const Node *n1, const Node *n2) const
        {
            return *n1 < *n2;
        }
    };
    // auto sort = [](Node *n1, Node *n2) -> bool {
    //     return *n1 < *n2;
    // };

public:
    Map(int a = 5, int b = 5, double wallDensity = 0.0, int _agents = 1);

    void initializeNodes(int rows, int cols);
    void initializeAllActions();
    void initializeAreas();
    void setRules(std::set<RuleRestrict> _rulesRestrict, std::set<RuleRequest> _rulesRequest = std::set<RuleRequest>{});

    void setWalls(double wallDensity);
    bool setWall(int x, int y);
    bool setFreeSpace(int x, int y);

    bool printMap();
    bool printMap(Node initial, std::list<Action> path = std::list<Action>());

    bool isInMap(int x, int y);
    bool isInMap(Node *n);

    void findNeighbours();

    Node *getNode(int x, int y) { return *nodes.find(new Node(x, y)); };
    Node *getNode(Node *n) { return *nodes.find(n); };
    // Node getNodeP(int x, int y) { return nodes.find(Node(x, y)); };

    std::map<Action, Node *> getNeighbours(Node *n);
    int getNumAreas() { return numAreas; };
    std::set<Action> getAllActions() { return allActions; };

    double distNode(Node n1, Node n2);
    int distNodeMoves(Node n1, Node n2);

    std::pair<double, Node> distTask(Node n, subtask_t subtask);
    int distMoves(Node n, subtask_t subtask);
    double dist(Node n1, Node n2, subtasks_t subtasks = subtasks_t{}, int label = 0);
    int distMoves(Node n1, Node n2, subtasks_t subtasks = subtasks_t{});

    bool isMissionValid(Node *start, Node *end);
    mission_t getMission(agentID_t agent) { return missions[agent]; };
    missions_t getMissions(std::set<agentID_t> agentSet);
    missions_t getMissions(agentID_t lastAgent);
    std::pair<Node *, Node *> getValidMission();
    missions_t getValidMissions(int n);
    bool doesMissionsFollowRules();
    void setMissions(missions_t _missions);

    std::set<Node *, Sorter> getNodes() { return nodes; };

    void newMap();

    std::map<Node *, int> areas{};

    void saveLevel(std::string name, paths_t paths = paths_t{});

private:
    int rows;
    int cols;
    double wallDensity;
    std::mt19937 mersenne;
    int agents;
    std::map<Node *, std::map<Action, Node *>> neighbours;
    // std::map<Node *, std::set<std::pair<Action, Node *>>> neighbours;
    std::set<Action> allActions;
    missions_t missions;
    std::set<Node *, Sorter> initialPos;
    std::set<Node *, Sorter> endPos;
    std::set<Node *, Sorter> nodes;
    std::set<RuleRestrict> rulesRestrict;
    std::set<RuleRequest> rulesRequest;

    int numAreas{1};
    std::map<Action, Node *> _getNeighbours(Node *n);
    // std::vector<std::vector<Node>> nodes;
};

#endif