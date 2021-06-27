#ifndef ASTAR_H
#define ASTAR_H

#include <set>
#include <list>
#include <functional>

#include "typeAliases.h"

#include "action.h"
#include "map.h"
#include "node.h"
#include "state.h"
#include "actionConstraint.h"
#include "ruleRequest.h"

class AStar
{
public:
    AStar(
        Node *startNode,
        Node *goalNode,
        agentID_t agent,
        Map *m,
        ACSet_t constraints = ACSet_t{},
        subtasks_t _subtasks = subtasks_t{},
        std::set<RuleRequest> *_reqRules = nullptr);
    ~AStar(){
        // if (reqRules->size() < 1)
        //     delete reqRules;
    };
    path_t search();
    std::function<bool(State *, State *)> getVisitedCompare();

    template <typename Set>
    void deleteStates(Set set);
    path_t copyPath(path_t path);

    subtasks_t subtasks;
    Node *start;
    Node *goal;
    Map *map;
    agentID_t agent;
    std::function<bool(State *, State *)> visitedCompare;
    std::set<ActionConstraint> constraints;
    std::set<RuleRequest> *reqRules;
    // std::map<
};

#endif