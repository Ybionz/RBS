#include "lightState.h"

LightState::LightState(Node *_current, Action _action, agentID_t _agent, int _g)
    : current{_current},
      action{_action},
      g{_g},
      agent{_agent} {};

bool LightState::operator<(const LightState &o) const
{
    if (*current < *o.current)
        return true;
    else if (*current > *o.current)
        return false;

    return action < o.action;
    // if (action < o.action)
    //     return true;
    // else if (action > o.action)
    //     return false;

    // if (g < o.g)
    //     return true;
    // else if (g > o.g)
    //     return false;

    // return agent < o.agent;
};

bool LightState::operator==(const LightState &o) const
{
    return (*current == *o.current && action == o.action); // && agent == o.agent);
    // if (*current < *o.current)
    //     return true;
    // else if (*current > *o.current)
    //     return false;

    // return action < o.action;
    // if (action < o.action)
    //     return true;
    // else if (action > o.action)
    //     return false;

    // if (g < o.g)
    //     return true;
    // else if (g > o.g)
    //     return false;

    // return agent < o.agent;
};

