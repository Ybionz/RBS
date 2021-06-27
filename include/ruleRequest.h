#ifndef RULEREQUEST_H
#define RULEREQUEST_H

#include <set>
#include <map>
#include <unordered_set>

#include "typeAliases.h"

#include "actionConstraint.h"
#include "lightState.h"
#include "map.h"
#include "state.h"

class RuleRequest
{
public:
    enum Type
    {
        sokoban,
        Last
    };
    RuleRequest(std::set<agentID_t> _affected,
                std::set<agentID_t> _helpers,
                Map *_map, int _count = 1,
                Type _type = Type::sokoban)
        : affected{_affected},
          helpers{_helpers},
          map{_map},
          count{_count},
          type{_type} {};

    std::vector<LightState> getRequest(LightState state) { return requests[state]; };
    bool cantGetHelp(Node n) const { return noHelp.contains(n); };
    bool isIllegal(Move m) const { return illegal.contains(m); };

    void makeSokoban();

    std::vector<LightState> getLSsforNode(Node node);

    bool operator<(const RuleRequest &o) const;
    Map *map;
    std::set<agentID_t> affected;
    std::set<agentID_t> helpers;
    std::unordered_map<LightState, std::vector<LightState>> requests{};
    std::unordered_set<Move> illegal{};
    std::unordered_set<Node> noHelp{};

    int count{1};
    Type type;
};

#endif