#ifndef HLSTATE_H
#define HLSTATE_H

#include "typeAliases.h"
#include "map.h"
#include "state.h"
#include "node.h"
#include "actionConstraint.h"
#include "aStar.h"

class HLState
{
public:
    HLState(tasks_t tasks, Map *map);

    HLState(const HLState &parent, constraints_t constraints);

    ACSet_t vertexToActionConstraints(int time, Node *vertex);

    template <typename Iterator>
    constraintSet_t getVertexConstraint(Iterator begin, Iterator end, int n);

    constraintSet_t getEdgeConstraint(const State *state, const State *opposite);

    constraintSet_t getConstraints();

    std::set<HLState *> getChildStates();

    bool isGoal();

    void findPaths();

    path_t getPath(agentID_t agent);
    paths_t getPaths();

    void addConstraints(constraints_t newConstraints);
    bool operator<(const HLState &o) const;
    int getG();
    int getSearchSuccess() { return searchSuccses; };

private:
    const HLState *parent;
    Map *map;
    tasks_t tasks;
    int numAgents;
    int g;
    constraints_t constraints;
    constraintSet_t childConstraints;
    paths_t paths;
    int cost;
    bool searchSuccses;
    bool foundConstraints;
    struct vertexCompare
    {
        bool operator()(const State *a, const State *b) const
        {
            return (*a->currentNode < *b->currentNode);
        };
    };
};

#endif