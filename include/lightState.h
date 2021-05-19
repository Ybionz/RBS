#ifndef LIGHTSTATE_H
#define LIGHTSTATE_H

#include "typeAliases.h"
#include "node.h"
#include "action.h"
// #include "node.h"

class LightState
{
    Node *current;
    Action action;
    int g;
    agentID_t agent;

public:
    LightState(Node *_current, Action _action, agentID_t _agent = -1, int _g = 0);
    ~LightState(){};

    int getG() { return g; };
    Node *getCurrent() const { return current; };
    Action getAction() const { return action; };
    agentID_t getAgent() const { return agent; };
    bool operator<(const LightState &o) const;
    bool operator==(const LightState &o) const;
};

namespace std
{
    template <>
    struct hash<LightState>
    {
        size_t operator()(const LightState &s) const
        {
            // Compute individual hash values for first, second and third
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash<Node>()(*s.getCurrent());
            res = res * 31 + hash<Action>()(s.getAction());
            return res;
        }
    };
}

#endif