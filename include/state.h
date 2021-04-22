#ifndef STATE_H
#define STATE_H

#include <set>
#include <list>

#include "node.h"
#include "map.h"
#include "action.h"
#include "actionConstraint.h"

class State
{
public:
    State(Node *initialNode, Node *goalNode, Map *map);

    State(const State &parent, Action action);

    std::set<State *> getChildStates(std::set<ActionConstraint> constraints);

    bool operator<(const State &o) const;

    bool atGoal();

    std::list<Action> getPath();

    Map *map;
    Node *currentNode;
    Node *initialNode;
    Node *goalNode;
    double g;
    double h;
    double f;
    Action action;
    const State *m_parent;
    bool isGoal;
};

#endif