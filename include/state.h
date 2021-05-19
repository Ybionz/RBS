#ifndef STATE_H
#define STATE_H

#include <set>
#include <list>
#include <utility>

#include "typeAliases.h"

#include "node.h"
#include "map.h"
#include "action.h"
#include "actionConstraint.h"
#include "lightState.h"

class State
{
public:
    State(Node *initialNode, Node *goalNode, int agent, Map *map);
    State(Node *current, Action action = Action::Direction::wait);
    ~State(){};

    State(const State &parent, const Action action);

    std::set<State *> getChildStates(ACSet_t constraints);
    friend std::ostream &operator<<(std::ostream &out, const State &n);

    bool operator<(const State &o) const;

    bool atGoal(ACSet_t aCSet);

    path_t getPath() const;
    std::list<Action> getPathActions() const;

    LightState getLS() const;

    Map *map;
    Node *currentNode;
    Node *initialNode;
    Node *goalNode;
    agentID_t agent;
    double g;
    double h;
    double f;
    Action action;
    const State *m_parent;
    bool isGoal;
};

#endif