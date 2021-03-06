#ifndef HLSTATE_H
#define HLSTATE_H

#include <vector>
#include <algorithm>
#include <iterator>

#include "typeAliases.h"
#include "map.h"
#include "state.h"
#include "node.h"
#include "actionConstraint.h"
#include "aStar.h"
#include "ruleRestrict.h"

class HLState
{
public:
    HLState(missions_t missions,
            Map *map,
            std::set<RuleRestrict> *_rulesRestrict = nullptr,
            std::set<RuleRequest> *_rulesRequest = nullptr);

    HLState(const HLState &parent,
            constraints_t constraints = constraints_t{},
            subtasks_t _subtask = subtasks_t{});

    ACSet_t vertexToActionConstraints(int time, Node *vertex);

    template <typename Iterator>
    constraintSet_t getConstraintRestrict(LightState main, Iterator begin, Iterator end, std::multiset<LightState> states, int k);

    template <typename Iterator>
    constraintSet_t getVertexConstraint(Iterator begin, Iterator end, int n);

    constraintSet_t getEdgeConstraint(const State *state, const State *opposite);

    constraintSet_t getConstraints();

    std::set<HLState *> getChildStates();

    bool isGoal();

    void findPaths();
    void padPaths();

    path_t getPath(agentID_t agent);
    paths_t getPaths();

    void addConstraints(constraints_t newConstraints);
    bool operator<(const HLState &o) const;
    int getG();
    int getSearchSuccess() { return searchSuccess; };
    int getLenght() { return length; };

private:
    const HLState *parent;
    Map *map;
    missions_t missions;
    int numAgents;
    int g;
    constraints_t constraints;
    constraintSet_t childConstraints;
    paths_t paths;
    int cost;
    bool searchSuccess{true};
    bool foundConstraints;
    int length;
    std::set<RuleRestrict> *rulesRestrict;
    std::set<RuleRequest> *rulesRequest;
    std::map<agentID_t, int> costs = std::map<agentID_t, int>{};
    std::set<agentID_t> gotConstraint;
    std::map<agentID_t, subtasks_t> tasks;
    struct vertexCompare
    {
        bool operator()(const State *a, const State *b) const
        {
            return (*a->currentNode < *b->currentNode);
        };
    };
};

#endif