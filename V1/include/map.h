#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector> // for std::vector
#include <random> // for std::mt19937
#include <ctime>  // for std::time
#include <list>   // for std::list
#include <map>    // for std::map
#include <set>    // for std::set
#include <cmath>  // for std::sqrt

#include "node.h"
#include "action.h"

class Map
{
public:
    Map(int a = 5, int b = 5, double wallDensity = 0.0);

    void initializeNodes(int rows, int cols);

    void setWalls(double wallDensity);

    bool setWall(int x, int y);

    bool printMap();

    bool isInMap(int x, int y);
    bool isInMap(Node *n);

    void findNeighbours();

    std::set<Action> getAllActions() { return allActions; };
    void initializeAllActions();

    Node *getNode(int x, int y) { return *nodes.find(new Node(x, y)); };
    Node *getNode(Node *n) { return *nodes.find(n); };
    // Node getNodeP(int x, int y) { return nodes.find(Node(x, y)); };

    std::set<Node *> getNeighbours(Node *n);

    double dist(Node n1, Node n2);

private:
    int rows;
    int cols;
    std::mt19937 mersenne;
    std::map<Node *, std::set<Node *>> neighbours;
    std::set<Node *> _getNeighbours(Node *n);
    std::set<Node *, decltype([](Node *a, Node *b) { return (*a < *b); })> nodes;
    std::set<Action> allActions;
    // std::vector<std::vector<Node>> nodes;
};

#endif