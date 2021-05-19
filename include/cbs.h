#ifndef CBS_H
#define CBS_H

#include <set>

#include "typeAliases.h"
#include "HLState.h"
#include "map.h"
#include "ruleRestrict.h"

class CBS
{
public:
    CBS(Map *m, std::set<agentID_t> _agents, std::set<RuleRestrict> _rules = std::set<RuleRestrict>{});

    paths_t search();

private:
    Map *map;
    std::set<agentID_t> agents;
    std::set<RuleRestrict> rules;
};

#endif