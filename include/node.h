#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "action.h"

class Node
{

public:
    Node(int a = 0, int b = 0, bool isWall = false);
    void makeWall();
    void makeFreeSpace();
    enum SpaceType
    {
        Wall,
        Free_space,
        Last
    };

    int getX() { return x; };
    int getY() { return y; };
    SpaceType getType() { return type; };

    bool operator==(const Node &o) { return (x == o.x && y == o.y); }
    bool operator!=(const Node &o) { return !(*this == o); }

    bool operator<(const Node &o) const;
    bool operator>(const Node &o) const;

    Node operator+(const Action &action) const;
    friend Node operator+(const Node &n, const Action &action);
    friend Node *operator+(Node *n, const Action &action);

    friend std::ostream &operator<<(std::ostream &out, const Node &n);

    void print();

private:
    int x;
    int y;
    SpaceType type;
};
#endif