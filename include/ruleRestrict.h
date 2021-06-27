#ifndef RULERESTRICT_H
#define RULERESTRICT_H

#include <map>
#include <set>

#include "typeAliases.h"

#include "actionConstraint.h"
#include "map.h"
#include "lightState.h"
#include "state.h"

class RuleRestrict
{
public:
    enum Type
    {
        distance,
        Last
    };
    RuleRestrict(std::set<agentID_t> _agents, Map *_map, int _count = 1, Type _type = Type::distance);

    // template <typename Iterator>
    // std::pair<Iterator, Iterator> getRestrict2(LightState state) const { return std::pair<Iterator, Iterator>{restrictions.find(state)->second.begin(), restrictions[state].end()}; }

    std::vector<LightState> getRestrict(LightState state) const { return restrictions.find(state)->second; };

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