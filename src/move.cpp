#include "move.h"

bool Move::operator<(const Move &o) const
{
    if (*vertex < *o.vertex)
        return true;
    else if (*vertex > *o.vertex)
        return false;

    return action < o.action;
}

bool Move::operator==(const Move &o) const
{
    return *vertex == *o.vertex && action == o.action;
}

Node *Move::result() const
{
    Node *res = new Node(*vertex + action);
    return res;
}