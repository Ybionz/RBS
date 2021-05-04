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

Action Action::opposite() const
{
    switch (dir)
    {
    case Action::Direction::wait:
        std::cout << "took opposite if wait\n";
        return Action(Direction::wait);
    case Action::Direction::north:
        return Action(Direction::south);
    case Action::Direction::south:
        return Action(Direction::north);
    case Action::Direction::west:
        return Action(Direction::east);
    case Action::Direction::east:
        return Action(Direction::west);
    default:
        return Action(Direction::wait);
    }
}