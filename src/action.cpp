#include "action.h"

std::ostream &operator<<(std::ostream &out, const Action &action)
{
    switch (action.dir)
    {
    case Action::Direction::wait:
        out << "wait";
        break;
    case Action::Direction::north:
        out << "north";
        break;
    case Action::Direction::south:
        out << "south";
        break;
    case Action::Direction::west:
        out << "west";
        break;
    case Action::Direction::east:
        out << "east";
        break;

    default:
        out << "action";
        break;
    }
    return out;
};

bool Action::operator<(const Action &o) const
{
    return dir < o.dir;
};

bool Action::operator>(const Action &o) const { return o < *this; }
