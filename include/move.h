#ifndef MOVE_H
#define MOVE_H

#include "typeAliases.h"

#include "node.h"
#include "action.h"

class Move
{
    Action action; // Action that has just been performed
    Node *vertex;  // Current node

public:
    Move(){};
    Move(Action _action, Node *_vertex) : action{_action}, vertex{_vertex} {};

    Action getAction() const { return action; };
    Node *getV() const { return vertex; };
    Node *getPrev() { return vertex - action; };

    Node *result() const;

    bool operator<(const Move &o) const;
    bool operator>(const Move &o) const { return o < *this; };
    bool operator==(const Move &o) const;
    bool operator!=(const Move &o) const { return !(*this == o); };
};

namespace std
{
    template <>
    struct hash<Move>
    {
        size_t operator()(const Move &m) const
        {
            // Compute individual hash values for first, second and third
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash<Node>()(*m.getV());
            res = res * 31 + hash<Action>()(m.getAction());
            return res;
        }
    };
}

#endif