#ifndef ACTIONCONSTRAINT_H
#define ACTIONCONSTRAINT_H

#include "action.h"
#include "node.h"

class ActionConstraint
{
public:
    ActionConstraint(Action action, int t, Node *n);

    bool operator<(const ActionConstraint &o) const;
    bool operator>(const ActionConstraint &o) const { return o < *this; };

    Action action;
    int t;
    Node *v;
};

#endif