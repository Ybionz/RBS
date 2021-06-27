#include "lightState.h"

LightState::LightState(Node *_current, Action _action, agentID_t _agent, int _g)
    : move{Move(_action, _current)},
      g{_g},
      agent{_agent} {};

bool LightState::operator<(const LightState &o) const
{
    return move < o.move;
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
    return move == o.move; // && agent == o.agent);
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
