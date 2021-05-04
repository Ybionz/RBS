#include "state.h"

State::State(Node *initial, Node *goal, int agent, Map *map)
    : map{map},
      initialNode{initial},
      currentNode{initial},
      goalNode{goal},
      agent{agent},
      g{0},
      h{(*map).dist(*initial, *goal)},
      f{g + h},
      action{Action::wait},
      isGoal{*currentNode == *goalNode},
      m_parent{nullptr} {};

State::State(const State &parent, Action action)
    : map{parent.map},
      currentNode{map->getNode(parent.currentNode + action)},
      initialNode{parent.initialNode},
      goalNode{parent.goalNode},
      agent{parent.agent},
      g{parent.g + 1.0},
      h{(*parent.map).dist(*currentNode, *goalNode)},
      f{g + h},
      action{action},
      isGoal{*currentNode == *goalNode},
      m_parent{&parent} {};

State::State(Node *current, const Action action)
    : currentNode{current},
      action{action} {};

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

path_t State::getPath() const
{

    if (m_parent == nullptr)
    {
        return path_t{this};
    }
    auto list = m_parent->getPath();
    list.push_back(this);
    return list;
    // std::list<const State *> temp;
    // temp.push_front(this);
    // const State *n = m_parent;
    // while (n != nullptr)
    // {
    //     temp.push_front(n);
    //     n = n->m_parent;
    // };
    // return temp;
}

std::list<Action> State::getPathActions() const
{
    std::list<Action> temp;
    const Action test{this->action};
    temp.push_front(test);
    const State *n = m_parent;
    while (n != nullptr)
    {
        bool testBool = n != NULL;
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