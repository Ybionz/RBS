#include "node.h"

Node::Node(int a, int b, bool isWall)
{
    x = a;
    y = b;
    type = isWall ? Wall : Free_space;
};

void Node::makeWall() { type = Wall; };
void Node::makeFreeSpace() { type = Free_space; };

void Node::print()
{
    std::cout << "Node at x = " << x << " and y = " << y << " has type " << type << '\n';
}

bool Node::operator<(const Node &o) const
{
    if (x < o.x)
    {
        return true;
    }
    else if (x > o.x)
    {
        return false;
    }
    if (y < o.y)
    {
        return true;
    }

    return false;
};
bool Node::operator>(const Node &o) const
{
    return o < *this;
};

Node Node::operator+(const Action &action) const
{

    // Node(0,0) is the top left corner and
    // Node(cols-1,rows-1) is in the bottom right corner
    switch (action.dir)
    {
    case Action::Direction::wait:
        return Node(x, y);
    case Action::Direction::north:
        return Node(x, y - 1);
    case Action::Direction::south:
        return Node(x, y + 1);
    case Action::Direction::west:
        return Node(x - 1, y);
    case Action::Direction::east:
        return Node(x + 1, y);

    default:
        return Node(x, y);
    }
};

Node operator+(const Node &n, const Action &action)
{

    switch (action.dir)
    {
    case Action::Direction::wait:
        return Node(n.x, n.y);
    case Action::Direction::north:
        return Node(n.x, n.y - 1);
    case Action::Direction::south:
        return Node(n.x, n.y + 1);
    case Action::Direction::west:
        return Node(n.x - 1, n.y);
    case Action::Direction::east:
        return Node(n.x + 1, n.y);

    default:
        return Node(n.x, n.y);
    }
};

Node *operator+(Node *n, const Action &action)
{
    switch (action.dir)
    {
    case Action::Direction::wait:
        return new Node(n->x, n->y);
    case Action::Direction::north:
        return new Node(n->x, n->y - 1);
    case Action::Direction::south:
        return new Node(n->x, n->y + 1);
    case Action::Direction::west:
        return new Node(n->x - 1, n->y);
    case Action::Direction::east:
        return new Node(n->x + 1, n->y);

    default:
        return new Node(n->x, n->y);
    }
};

Node *operator-(Node *n, const Action &action)
{
    switch (action.dir)
    {
    case Action::Direction::wait:
        return new Node(n->x, n->y);
    case Action::Direction::north:
        return new Node(n->x, n->y + 1);
    case Action::Direction::south:
        return new Node(n->x, n->y - 1);
    case Action::Direction::west:
        return new Node(n->x + 1, n->y);
    case Action::Direction::east:
        return new Node(n->x - 1, n->y);

    default:
        return new Node(n->x, n->y);
    }
};

std::ostream &operator<<(std::ostream &out, const Node &n)
{
    out << "Node(" << n.x << ", " << n.y << ')';

    return out;
}

