#include "state.h"

State::State(Node *initial, Node *goal, int agent, Map *map, subtasks_t _subtasks)
    : map{map},
      m_parent{nullptr},
      initialNode{initial},
      currentNode{initial},
      goalNode{goal},
      agent{agent},
      subtasks{_subtasks},
      g{0},
      h{(*map).dist(*initial, *goal, subtasks, subtaskLabel)},
      f{g + h},
      action{Action::wait},
      isGoal{subtaskLabel == subtasks.size() && *currentNode == *goalNode} {};

State::State(const State &parent, Action action)
    : m_parent{&parent},
      map{parent.map},
      currentNode{map->getNode(parent.currentNode + action)},
      initialNode{parent.initialNode},
      goalNode{parent.goalNode},
      subtasks{parent.subtasks},
      subtaskLabel{parent.subtaskLabel},
      agent{parent.agent},
      g{parent.g + 1.0},
      h{(*parent.map).dist(*currentNode, *goalNode, subtasks, subtaskLabel)},
      f{g + h},
      action{action},
      isGoal{subtaskLabel == subtasks.size() && *currentNode == *goalNode}
{
    if (updateSTLabel())
        h = (*parent.map).dist(*currentNode, *goalNode, subtasks, subtaskLabel);
};

State::State(Node *current, const Action action)
    : currentNode{current},
      action{action} {

      };

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

bool State::atGoal(ACSet_t aCSet)
{
    if (!isGoal)
        return isGoal;

    if (aCSet.size() < 1)
        return isGoal;

    if (aCSet.rbegin()->t >= g)
        return false;
    return isGoal;
};

std::set<State *> State::getChildStates(ACSet_t aCSet, std::set<RuleRequest> *reqRules)
{
    std::set<State *> children;
    for (auto [a, n] : (*map).getNeighbours(currentNode))
    {
        bool actionPermitted{aCSet.find(ActionConstraint(a, static_cast<int>(g), currentNode)) == aCSet.end()};
        if (reqRules != nullptr)
        {
            for (auto const &rule : *reqRules)
            {
                if (rule.isIllegal(Move(a, currentNode + a)))
                    goto nextNeighbour; // continue on outer loop
                if (rule.cantGetHelp(*(currentNode + a)))
                    goto nextNeighbour; // continue on outer loop
            }
        }
        if (actionPermitted)
        {
            State *child = new State{*this, a};
            children.insert(child);
        }
    nextNeighbour:;
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

LightState State::getLS() const
{
    return LightState(currentNode, action, agent, g);
};

Move State::getMove() const
{
    return Move(action, currentNode - action);
}

bool State::updateSTLabel()
{
    if (subtasks.size() == subtaskLabel)
        return false;
    // Move move{getMove()};
    LightState ls{getLS()};
    auto begin = subtasks[subtaskLabel].begin();
    auto end = subtasks[subtaskLabel].end();
    if (std::lower_bound(begin, end, ls) != end)
    {
        subtaskLabel++;
        return true;
    }
    return false;
}