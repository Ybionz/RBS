#include "HLState.h"

HLState::HLState(tasks_t tasks, Map *map)
    : parent{nullptr},
      map{map},
      tasks{tasks},
      numAgents{static_cast<int>(tasks.size())},
      cost{0},
      g{0},
      foundConstraints{false},
      constraints{constraints_t{}},
      searchSuccses{true}
{
    findPaths();
};

HLState::HLState(const HLState &_parent, constraints_t _constraints)
    : parent{&_parent},
      map{_parent.map},
      constraints{_parent.constraints},
      tasks{_parent.tasks},
      numAgents{_parent.numAgents},
      cost{0},
      g{_parent.g + 1},
      foundConstraints{false},
      searchSuccses{true}

{
    addConstraints(_constraints);
    findPaths();
};

int HLState::getG()
{
    return g;
}

void HLState::addConstraints(constraints_t newConstraints)
{
    for (auto [agent, aCons] : newConstraints)
    {
        constraints[agent].insert(aCons.begin(), aCons.end());
    }
}

path_t HLState::getPath(agentID_t agent)
{
    return paths[agent];
}

void HLState::findPaths()
{
    for (auto [agent, task] : tasks)
    {
        // if no parent find new path for all agents with no constraints
        if (parent == nullptr)
        {
            paths[agent] = AStar(task.first, task.second, agent, map).search();
            cost += paths[agent].size();
            continue;
        }
        else
        {
            // if no new constraint for this agent get path from parent
            if (constraints.find(agent) == parent->constraints.find(agent))
            {
                paths_t pPaths = parent->paths;
                paths[agent] = pPaths[agent];
                cost += paths[agent].size();
                continue;
            }
            paths[agent] = AStar(task.first, task.second, agent, map, constraints[agent]).search();
            if (paths[agent].size() < 1)
                searchSuccses = false;
            cost += paths[agent].size();
        }
    }
}

ACSet_t HLState::vertexToActionConstraints(int time, Node *vertex)
{
    ACSet_t actionConstraints;
    for (int dir{Action::Direction::east}; dir != Action::Direction::Last; ++dir)
    {
        Action a{static_cast<Action::Direction>(dir)};
        actionConstraints.insert(ActionConstraint{a, time, vertex - a});
    }
    return actionConstraints;
};

template <typename Iterator>
constraintSet_t HLState::getVertexConstraint(Iterator begin, Iterator end, int n)
{
    Iterator current{begin};
    constraintSet_t constraints;
    for (int i{0}; i < n; ++i)
    {
        constraints_t childConstraints;
        const State *state{*current};
        int agent{state->agent};
        Iterator copyBegin{begin};

        for (auto it = begin; it != end; ++it)
        {
            const State *otherState = *it;
            if (otherState == state)
                continue;
            std::cout << "VertexConflict at " << *otherState->currentNode << '\n';
            childConstraints[otherState->agent] = vertexToActionConstraints(otherState->g - 1, otherState->currentNode);
        }
        copyBegin = begin;
        constraints.insert(childConstraints);
        current++;
    }
    return constraints;
};

constraintSet_t HLState::getEdgeConstraint(const State *state, const State *opposite)
{

    ACSet_t agentConstraints1{ActionConstraint(state->action, state->g - 1, state->currentNode - state->action)};
    constraints_t childConstraints1{{state->agent, agentConstraints1}};

    ACSet_t agentConstraints2{ActionConstraint(opposite->action, opposite->g - 1, opposite->currentNode - opposite->action)};
    constraints_t childConstraints2{{opposite->agent, agentConstraints2}};

    constraintSet_t constraints{childConstraints1, childConstraints2};
    return constraints;
};

constraintSet_t HLState::getConstraints()
{
    foundConstraints = true;
    // Paths: agentID Path
    auto edgeConSort{[](const State *a, const State *b) {
        if (*a->currentNode < *b->currentNode)
            return true;
        else if (*a->currentNode > *b->currentNode)
            return false;

        return a->action < b->action;
    }};

    // statesAtTime: tiomestep, set of statesstatesEdgeCon[i].count(state)
    std::map<int, std::multiset<const State *, decltype(edgeConSort)>> statesEdgeCon;
    std::map<int, std::multiset<const State *,
                                vertexCompare>>
        statesVertexCon;
    int maxLength{0};
    for (auto [agent, states] : paths)
    {
        maxLength = std::max(maxLength, static_cast<int>(states.size()));
        for (const State *s : states)
        {
            statesVertexCon[s->g].insert(s);
            statesEdgeCon[s->g].insert(s);
        }
    }
    // auto iter;
    // for each time step check if any conflicts occur
    for (int i{0}; i < maxLength; ++i)
    {
        for (auto state : statesVertexCon[i])
        {
            int vCount{static_cast<int>(statesVertexCon[i].count(state))};
            if (vCount > 1)
            {
                auto iter{statesVertexCon[i].equal_range(state)};
                return getVertexConstraint(iter.first, iter.second, vCount);
            }
        }
        for (auto state : statesEdgeCon[i])
        {
            if (state->m_parent == nullptr || state->action.dir == Action::Direction::wait)
                continue; // skip initialState
            const State *opposite = new State(state->m_parent->currentNode, state->action.opposite());
            int eCount{static_cast<int>(statesEdgeCon[i].count(opposite))};
            if (eCount > 0)
            {
                auto iter{statesEdgeCon[i].equal_range(state)};
                auto oppositeS = *iter.first;
                return getEdgeConstraint(state, oppositeS);
            }
        }
    }
    return constraintSet_t{};
};

bool HLState::isGoal()
{
    if (!foundConstraints)
        childConstraints = getConstraints();
    return (childConstraints.size() > 0) ? false : true;
};

std::set<HLState *> HLState::getChildStates()
{
    std::set<HLState *> children;

    if (!foundConstraints)
        childConstraints = getConstraints();

    for (auto newConst : childConstraints)
    {
        HLState *child = new HLState(*this, newConst);
        if (child->getSearchSuccess())
            children.insert(child);
    }
    return children;
};

bool HLState::operator<(const HLState &o) const
{
    if (cost < o.cost)
        return true;
    else if (cost > o.cost)
        return false;

    if (paths < o.paths)
        return true;

    return false;
};

paths_t HLState::getPaths()
{
    return paths;
}