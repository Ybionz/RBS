#include "actionConstraint.h"

ActionConstraint::ActionConstraint(Action action, int t, Node *n)
    : action{action},
      t{t},
      v{n} {};

bool ActionConstraint::operator<(const ActionConstraint &o) const
{
  if (t < o.t)
    return true;
  else if (t > o.t)
    return false;

  if (*v < *o.v)
    return true;
  else if (*v > *o.v)
    return false;

  if (action < o.action)
    return true;

  return false;
};

std::ostream &operator<<(std::ostream &out, const ActionConstraint &a)
{
  out << "ActionConstraint(t=" << a.t << ", v=" << *a.v << ", action=" << a.action << ")";

  return out;
};