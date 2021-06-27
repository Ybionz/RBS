#include "ruleRestrict.h"

RuleRestrict::RuleRestrict(std::set<agentID_t> _agents, Map *_map, int _count, Type _type)
    : agents{_agents},
      type{_type},
      count{_count},
      map{_map} {};

std::vector<LightState> RuleRestrict::getLSsforNode(Node node)
{
    std::vector<LightState> temp;
    for (auto [action, neigh] : map->getNeighbours(&node))
    {
        if (action.isWait())
            temp.push_back(LightState(map->getNode(&node), action));
        else
            temp.push_back(LightState(map->getNode(&node), action.opposite()));
    }
    return temp;
};

bool RuleRestrict::operator<(const RuleRestrict &o) const
{
    return type < o.type;
};

void RuleRestrict::makeDistance(int dist)
{
    restrictions.clear();
    type = Type::distance;
    for (auto node : map->getNodes())
    {
        for (auto [action, neigh] : map->getNeighbours(node))
        {
            LightState current(node, action.opposite());
            std::set<Node> open;
            std::set<Node> closed;
            open.insert(*node);
            while (!open.empty())
            {
                Node nextNode = open.extract(open.begin()).value();
                closed.insert(nextNode);
                int depth = map->distMoves(*node, nextNode);
                if (depth > dist || nextNode.getType() == Node::SpaceType::Wall)
                    continue;

                auto temp = getLSsforNode(nextNode);
                restrictions[current].insert(restrictions[current].end(), temp.begin(), temp.end());

                for (auto [nextAction, next] : map->getNeighbours(&nextNode))
                {
                    if (open.find(*next) == open.end() && closed.find(*next) == closed.end())
                    {
                        open.insert(*next);
                    }
                }
            }
            std::sort(restrictions[current].begin(), restrictions[current].end());
        }
    }
};
