#include "HLState.h"

HLState::HLState(missions_t missions,
                 Map *map,
                 std::set<RuleRestrict> *_rulesRestrict,
                 std::set<RuleRequest> *_rulesRequest)
    : parent{nullptr},
      map{map},
      missions{missions},
      numAgents{static_cast<int>(missions.size())},
      cost{0},
      g{0},
      foundConstraints{false},
      constraints{constraints_t{}},
      length{0},
      rulesRequest{_rulesRequest},
      rulesRestrict{_rulesRestrict}
{
    findPaths();
};

HLState::HLState(const HLState &_parent, constraints_t _constraints, subtasks_t _subtasks)
    : parent{&_parent},
      map{_parent.map},
      constraints{_parent.constraints},
      missions{_parent.missions},
      numAgents{_parent.numAgents},
      cost{0},
      g{_parent.g + 1},
      foundConstraints{false},
      length{_parent.length},
      rulesRequest{_parent.rulesRequest},
      rulesRestrict{_parent.rulesRestrict}

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
        gotConstraint.insert(agent);
        for (auto c : aCons)
        {
            auto temp = constraints[agent].insert(c);
            if (!temp.second)
            {
                std::cout << "faild to insert\n";
            }
        }
        // constraints[agent].insert(aCons.begin(), aCons.end());
    }
}

path_t HLState::getPath(agentID_t agent)
{
    return paths[agent];
}

void HLState::padPaths()
{
    int agents{static_cast<int>(paths.size())};
    for (agentID_t agent{0}; agent < agents; agent++)
        while (paths[agent].size() < length)
        {
            auto last = paths[agent].back();
            paths[agent].push_back(new State(*last, Action(Action::Direction::wait)));
        };
};

void HLState::findPaths()
{
    for (auto [agent, mission] : missions)
    {
        // if no parent find new path for all agents with no constraints
        if (parent == nullptr)
        {
            paths[agent] = AStar(mission.first, mission.second, agent, map,ACSet_t{},subtasks_t{},rulesRequest).search();
            costs[agent] = paths[agent].size();
            // cost += paths[agent].size();
            length = std::max(static_cast<int>(paths[agent].size()), length);
            continue;
        }
        else
        {
            // if no new constraint for this agent get path from parent
            if (!gotConstraint.contains(agent))
            {
                paths_t pPaths = parent->paths;
                auto pCosts = parent->costs;
                paths[agent] = pPaths[agent];
                costs[agent] = pCosts[agent];
                length = std::max(static_cast<int>(paths[agent].size()), length);
                continue;
            }
            paths[agent] = AStar(mission.first, mission.second, agent, map, constraints[agent]).search();
            if (paths[agent].size() < 1)
                searchSuccess = false;
            costs[agent] = paths[agent].size();
            length = std::max(static_cast<int>(paths[agent].size()), length);
        }
    }
    padPaths();
    for (auto [agent, co] : costs)
    {
        cost += co;
    };
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
constraintSet_t HLState::getConstraintRestrict(LightState main, Iterator begin, Iterator end, std::multiset<LightState> states, int k)
{
    ACSet_t acForMain{ActionConstraint(main.getAction(), main.getG() - 1, main.getCurrent() - main.getAction())};
    constraints_t mainAgentCon{{main.getAgent(), acForMain}};
    // constraints_t mainAgentCon{{main.getAgent(), vertexToActionConstraints(main.getG() - 1, main.getCurrent())}};
    constraintSet_t temp{mainAgentCon};
    std::vector<LightState> conStates;
    constraints_t others;
    while (begin != end)
    {
        LightState next{*begin};
        auto equal = states.equal_range(next);
        conStates.insert(conStates.end(), equal.first, equal.second);
        begin++;
    }
    for (auto state : conStates)
    {
        if (state.getAgent() == main.getAgent())
        {
            continue;
        }
        // others[state.getAgent()] = vertexToActionConstraints(state.getG() - 1, state.getCurrent());
        others[state.getAgent()] = ACSet_t{ActionConstraint(state.getAction(), state.getG() - 1, state.getCurrent() - state.getAction())};
    }
    temp.insert(others);
    return temp;
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
            // std::cout << "VertexConflict at " << *otherState->currentNode << '\n';
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
    // auto edgeConSort{[](const State *a, const State *b) {
    //     if (*a->currentNode < *b->currentNode)
    //         return true;
    //     else if (*a->currentNode > *b->currentNode)
    //         return false;

    //     return a->action < b->action;
    // }};

    // statesAtTime: tiomestep, set of statesstatesEdgeCon[i].count(state)
    std::map<int, std::multiset<LightState>> statesAtTime;
    std::map<int, std::vector<LightState>> statesAtTimeV;
    // std::map<int, std::multiset<const State *, decltype(edgeConSort)>> statesEdgeCon;
    // std::map<int, std::multiset<const State *,
    //                             vertexCompare>>
    //     statesVertexCon;
    for (auto [agent, states] : paths)
    {
        for (const State *s : states)
        {
            // statesVertexCon[s->g].insert(s);
            // statesEdgeCon[s->g].insert(s);
            statesAtTime[s->g].insert(s->getLS());
            statesAtTimeV[s->g].push_back(s->getLS());
        }
    }
    for (int i{0}; i < length; ++i)
    {
        std::sort(statesAtTimeV[i].begin(), statesAtTimeV[i].end());
    }
    // auto iter;
    // for each time step check if any conflicts occur
    for (int i{0}; i < length; ++i)
    {
        for (auto state : statesAtTime[i])
        {
            auto sATbegin = statesAtTimeV[i].begin();
            auto sATend = statesAtTimeV[i].end();
            for (auto const &rule : *rulesRestrict)
            {
                if (!rule.agents.contains(state.getAgent()))
                    continue;
                std::vector<LightState> inter{};
                // auto start = rule.restrictions[state].begin();
                // auto end = rule.restrictions[state].end();
                auto set = rule.getRestrict(state);
                auto start = set.begin();
                auto end = set.end();
                std::set_intersection(sATbegin, sATend, start, end, std::back_inserter(inter));
                int count{0};
                for (auto vertex : inter)
                {
                    count += statesAtTime[i].count(vertex);
                }
                if (count > rule.count)
                {
                    return getConstraintRestrict(state, inter.begin(), inter.end(), statesAtTime[i], rule.count);
                };
            }
        }
        // for (auto state : statesVertexCon[i])
        // {

        //     int vCount{static_cast<int>(statesVertexCon[i].count(state))};
        //     if (vCount > 1)
        //     {
        //         auto iter{statesVertexCon[i].equal_range(state)};
        //         return getVertexConstraint(iter.first, iter.second, vCount);
        //     }
        // }
        // for (auto state : statesEdgeCon[i])
        // {
        //     if (state->m_parent == nullptr || state->action.dir == Action::Direction::wait)
        //         continue; // skip initialState
        //     const State *opposite = new State(state->currentNode - state->action, state->action.opposite());
        //     int eCount{static_cast<int>(statesEdgeCon[i].count(opposite))};
        //     if (eCount > 0)
        //     {
        //         auto iter{statesEdgeCon[i].equal_range(state)};
        //         auto oppositeS = *iter.first;
        //         return getEdgeConstraint(state, oppositeS);
        //     }
        //     delete opposite;
        // }
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