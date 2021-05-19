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

    int getX() const { return x; };
    int getY() const { return y; };
    SpaceType getType() { return type; };

    bool operator==(const Node &o) { return (x == o.x && y == o.y); }
    bool operator!=(const Node &o) { return !(*this == o); }

    bool operator<(const Node &o) const;
    bool operator>(const Node &o) const;

    Node operator+(const Action &action) const;
    friend Node operator+(const Node &n, const Action &action);
    friend Node *operator+(Node *n, const Action &action);
    friend Node *operator-(Node *n, const Action &action);

    friend std::ostream &operator<<(std::ostream &out, const Node &n);

    void print();

private:
    int x;
    int y;
    SpaceType type;
};

namespace std
{
    template <>
    struct hash<Node>
    {
        size_t operator()(const Node &n) const
        {
            // Compute individual hash values for first, second and third
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash<int>()(n.getX());
            res = res * 31 + hash<int>()(n.getY());
            return res;
        }
    };
}

#endif