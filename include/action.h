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
    Action(Action::Direction dir) : dir{dir} {};

    Direction getDir() { return dir; };

    Direction dir;

    Action opposite() const ;
    // const Action opposite();

    friend std::ostream &operator<<(std::ostream &out, const Action &action);

    bool operator<(const Action &o) const;
    bool operator>(const Action &o) const;
};

#endif