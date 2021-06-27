#ifndef STATE_H
#define STATE_H

#include <algorithm>
#include <list>
#include <set>
#include <utility>

#include "typeAliases.h"

#include "action.h"
#include "actionConstraint.h"
#include "lightState.h"
#include "map.h"
#include "move.h"
#include "node.h"
#include "ruleRequest.h"

class State
{
public:
    State(Node *initialNode, Node *goalNode, int agent, Map *map, subtasks_t _subtasks = subtasks_t{});
    State(Node *current, Action action = Action::Direction::wait);
    ~State(){};

    State(const State &parent, const Action action);

    std::set<State *> getChildStates(ACSet_t constraints, std::set<RuleRequest> *reqRules);
    friend std::ostream &operator<<(std::ostream &out, const State &n);

    bool operator<(const State &o) const;

    bool atGoal(ACSet_t aCSet);

    path_t getPath() const;
    std::list<Action> getPathActions() const;

    LightState getLS() const;

    Move getMove() const; // returns the move that has just been made

    bool updateSTLabel();

    const State *m_parent;
    Map *map;
    Node *currentNode;
    Node *initialNode;
    Node *goalNode;
    agentID_t agent;
    subtasks_t subtasks;
    int subtaskLabel{0};
    double g;
    double h;
    double f;
    Action action;
    bool isGoal;
};

#endif