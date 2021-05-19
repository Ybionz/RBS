#include "cbs.h"

CBS::CBS(Map *m, std::set<agentID_t> _agents, std::set<RuleRestrict> _rules)
    : map{m},
      agents{_agents},
      rules{_rules} {};

paths_t CBS::search()
{
    missions_t missions = map->getMissions(agents);
    // mission_t first{map->getNode(0, 1), map->getNode(6, 1)};
    // mission_t second{map->getNode(6, 3), map->getNode(0, 3)};
    // missions_t missions{{0, first}, {1, second}};
    // map->setMissions(missions);

    HLState *initial = new HLState(missions, map, rules);

    std::multiset<HLState *, decltype([](HLState *a, HLState *b)
                                      { return (*a < *b); })>
        open;
    std::multiset<HLState *, decltype([](HLState *a, HLState *b)
                                      { return (*a < *b); })>
        closed;

    open.insert(initial);

    HLState *nextState;
    paths_t paths;
    while (!open.empty())
    {
        nextState = open.extract(open.begin()).value();
        closed.insert(nextState);

        if (nextState->isGoal())
        {
            return nextState->getPaths();
        };

        for (HLState *s : nextState->getChildStates())
        {
            // if (visited.find(s) == visited.end())
            if (open.find(s) == open.end() && closed.find(s) == closed.end())
            {
                // visited.insert(s);
                open.insert(s);
            }
        };
        if (closed.size() % 10 == 1)
        {
            std::cout << "In CBS closed.size() = " << closed.size() << " and open.size() = " << open.size() << '\n';
        }
    }
    paths_t failed{};
    return failed;
}