#include "ruleRequest.h"

void RuleRequest::makeSokoban()
{
    requests.clear();
    type = Type::sokoban;
    for (auto node : map->getNodes())
    {
        auto neighs{map->getNeighbours(node)};
        bool canBeMoved{false};
        for (auto [action, neigh] : neighs)
        {
            if (action.isWait())
                continue;
            std::pair<Action, Node *> temp;
            if (neighs.find(action.opposite()) != neighs.end())
            {
                LightState ins{node, action};
                LightState req{node - action.opposite(), action};
                requests[ins] = std::vector<LightState>{req};
                canBeMoved = true;
            }
            else
            {
                illegal.insert(Move{action, neigh});
            }
        }
        if (!canBeMoved)
        {
            noHelp.insert(*node);
        }
    }
};

bool RuleRequest::operator<(const RuleRequest &o) const
{
    return type < o.type;
}