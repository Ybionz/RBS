#include "action.h"

std::ostream &operator<<(std::ostream &out, const Action &action)
{
    switch (action.dir)
    {
    case Action::Direction::wait:
        out << "WAIT";
        break;
    case Action::Direction::north:
        out << "NORTH";
        break;
    case Action::Direction::south:
        out << "SOUTH";
        break;
    case Action::Direction::west:
        out << "WEST";
        break;
    case Action::Direction::east:
        out << "EAST";
        break;

    default:
        out << "ACTION";
        break;
    }
    return out;
};

bool Action::operator<(const Action &o) const
{
    return dir < o.dir;
};

bool Action::operator==(const Action &o) const
{
    return dir == o.dir;
};

bool Action::operator>(const Action &o) const { return o < *this; }

Action Action::opposite() const
{
    switch (dir)
    {
    case Action::Direction::wait:
        // std::cout << "took opposite if wait\n";
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