#ifndef ACTION_H
#define ACTION_H

#include <iostream>

class Action
{
public:
    enum Direction
    {
        east,
        north,
        south,
        wait,
        west,
        Last
    };
    Action(){};
    Action(Action::Direction dir) : dir{dir} {};

    Direction getDir() const { return dir; };

    Action opposite() const;

    bool isWait() const { return dir == Direction::wait; };
    // const Action opposite();

    friend std::ostream &operator<<(std::ostream &out, const Action &action);

    bool operator<(const Action &o) const;
    bool operator>(const Action &o) const;
    bool operator==(const Action &o) const;
    Direction dir;
};

namespace std
{
    template <>
    struct hash<Action>
    {
        size_t operator()(const Action &a) const
        {
            // Compute individual hash values for first, second and third
            // http://stackoverflow.com/a/1646913/126995
            return a.getDir();
        }
    };
}

#endif