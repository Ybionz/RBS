#ifndef RULERESTRICT_H
#define RULERESTRICT_H

#include <set>
#include <map>

#include "typeAliases.h"
#include "actionConstraint.h"
#include "state.h"
#include "lightState.h"
#include "map.h"

class RuleRestrict
{
public:
    enum Type
    {
        distance,
        Last
    };
    RuleRestrict(std::set<agentID_t> _agents, Map *_map, int _count = 1, Type _type = Type::distance);

    std::vector<LightState> getRestrict(LightState state) { return restrictions[state]; };

    void makeDistance(int dist = 0);
    std::vector<LightState> getLSsforNode(Node node);

    bool operator<(const RuleRestrict &o) const;
    Map *map;
    std::set<agentID_t> agents;
    std::unordered_map<LightState, std::vector<LightState>> restrictions{};
    int count{1};
    Type type;
};

#endif