#include "state.h"

State::State(Node *initial, Node *goal, Map *map)
    : initialNode{initial},
      currentNode{initial},
      goalNode{goal},
      map{map},
      g{0},
      h{(*map).dist(*initial, *goal)},
      f{g + h},
      action{Action::wait},
      isGoal{*currentNode == *goalNode},
      m_parent{nullptr} {};

State::State(const State &parent, Action action)
    : map{parent.map},
      currentNode{map->getNode(parent.currentNode) + action},
      initialNode{parent.initialNode},
      goalNode{parent.goalNode},
      g{parent.g + 1.0},
      h{(*parent.map).dist(*currentNode, *goalNode)},
      f{g + h},
      action{action},
      isGoal{*currentNode == *goalNode},
      m_parent{&parent} {};

bool State::operator<(const State &o) const
{
    if (f < o.f)
        return true;
    else if (f > o.f)
        return false;

    if (h < o.h)
        return true;
    else if (h > o.h)
        return false;

    if (action < o.action)
        return true;
    else if (action > o.action)
        return false;

    return currentNode < o.currentNode;
};

bool State::atGoal()
{
    return isGoal;
};

std::set<State *> State::getChildStates(std::set<ActionConstraint> constraints)
{
    std::set<State *> children;
    for (auto [a, n] : (*map).getNeighbours(currentNode))
    {
        bool actionPermitted{constraints.find(ActionConstraint(a, static_cast<int>(g), currentNode)) == constraints.end()};
        // bool notWall{map->getNode(n)->getType() != Node::SpaceType::Wall};
        if (actionPermitted)
        {
            State *child = new State{*this, a};
            children.insert(child);
        }
    };
    return children;
};

std::list<Action> State::getPath()
{
    std::list<Action> temp;
    temp.push_front(action);
    const State *n = m_parent;
    while (n != nullptr)
    {
        temp.push_front(n->action);
        n = n->m_parent;
    };
    return temp;
}

std::ostream &operator<<(std::ostream &out, const State &s)
{
    out << "currentNode: " << *s.currentNode << ", action:" << s.action << ", g=" << s.g << ", h=" << s.h << ", f=" << s.f << '\n';

    return out;
}