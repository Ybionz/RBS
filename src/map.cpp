#include "map.h"

Map::Map(int a, int b, double wallDensity)
    : cols{a},
      rows{b},
      wallDensity{wallDensity},
      mersenne{static_cast<std::mt19937::result_type>(std::time(nullptr))}
{
    initializeNodes(rows, cols);
    setWalls(wallDensity);
    initializeAllActions();
    findNeighbours();
    initializeAreas();
};

void Map::initializeNodes(int rows, int cols)
{
    for (int i{0}; i < cols; ++i)
    {
        for (int j{0}; j < rows; ++j)
        {
            nodes.insert(new Node(i, j));
        }
    }
};

void Map::setWalls(double wallDensity)
{
    // std::mt19937 mersenne{static_cast<std::mt19937::result_type>(std::time(nullptr))};
    std::uniform_int_distribution randomX{0, cols - 1};
    std::uniform_int_distribution randomY{0, rows - 1};

    double currentWallDensity{0.0};
    double oneWall{1.0 / (rows * cols)};

    while (currentWallDensity < wallDensity)
    {
        int x = randomX(mersenne);
        int y = randomY(mersenne);
        if (setWall(x, y))
        {
            currentWallDensity += oneWall;
        }
    }
};

bool Map::setWall(int x, int y)
{
    if (getNode(x, y)->getType() == Node::SpaceType::Wall)
    {
        return false;
    }
    getNode(x, y)->makeWall();
    return true;
};
bool Map::setFreeSpace(int x, int y)
{
    if (getNode(x, y)->getType() == Node::SpaceType::Free_space)
    {
        return false;
    }
    getNode(x, y)->makeFreeSpace();
    return true;
};

bool Map::printMap()
{
    // Node a{rows, cols};
    return printMap(Node(rows, cols));
}

bool Map::printMap(Node initial, std::list<Action> path)
{
    // auto it = path.begin();
    // it++; // because first action is wait
    // Node current{*getNode(initial.getX(), initial.getY())};
    std::set<Node> pathNodes{initial};
    Node current{initial};
    for (auto const &a : path)
    {
        current = current + a;
        pathNodes.insert(current);
    }

    for (int i{0}; i < rows; ++i)
    {
        for (int j{0}; j < cols; ++j)
        {
            if (getNode(j, i)->getType() == Node::SpaceType::Wall)
            {
                std::cout << char(0xdb);
            }
            else if (pathNodes.contains(*getNode(j, i)))
            {
                std::cout << "x";
            }
            else
            {
                std::cout << ".";
            }
        }
        std::cout << '\n';
    }
    return true;
};

bool Map::isInMap(int x, int y)
{
    return (x < cols && y < rows && x >= 0 && y >= 0) ? true : false;
};

bool Map::isInMap(Node *n) { return Map::isInMap(n->getX(), n->getY()); };

std::set<std::pair<Action, Node *>> Map::_getNeighbours(Node *n)
{
    std::set<std::pair<Action, Node *>> neighbours{};
    int x{n->getX()};
    int y{n->getY()};
    for (Action a : allActions)
    {
        Node *neigh = n + a;
        if (isInMap(neigh) && getNode(neigh)->getType() != Node::SpaceType::Wall)
        {
            neighbours.insert(std::pair<Action, Node *>{a, neigh});
        }
    }

    return neighbours;
};

std::set<std::pair<Action, Node *>> Map::getNeighbours(Node *n)
{
    return neighbours[getNode(n)];
};

void Map::findNeighbours()
{
    neighbours.clear();
    for (int i{0}; i < cols; i++)
    {
        for (int j{0}; j < rows; j++)
        {
            neighbours[getNode(i, j)] = _getNeighbours(getNode(i, j));
        }
    }
};

double Map::dist(Node n1, Node n2)
{
    int x{abs(n1.getX() - n2.getX())};
    int y{abs(n1.getY() - n2.getY())};
    return std::sqrt(x * x + y * y);
};

void Map::initializeAllActions()
{
    for (int i = Action::Direction::east; i != Action::Direction::Last; ++i)
    {
        allActions.insert(Action(static_cast<Action::Direction>(i)));
    }
};

void Map::initializeAreas()
{
    areas.clear();
    std::set<Node> nodesLeft;
    for (auto n : nodes)
    {
        nodesLeft.insert(*n);
    }
    int i{1};
    Node n;
    while (!nodesLeft.empty())
    {
        n = *nodesLeft.begin();
        nodesLeft.extract(n);
        if (n.getType() == Node::SpaceType::Wall)
        {
            areas[getNode(&n)] = 0;
            continue;
        }
        // std::set<Node *, decltype([](Node *a, Node *b) { return (*a < *b); })> open;
        // std::set<Node *, decltype([](Node *a, Node *b) { return (*a < *b); })> closed;
        std::set<Node> open;
        std::set<Node> closed;
        open.insert(n);
        Node nextNode;
        while (!open.empty())
        {
            nextNode = open.extract(open.begin()).value();
            nodesLeft.extract(nextNode);
            areas[getNode(&nextNode)] = i;
            for (auto [a, neighbour] : getNeighbours(&nextNode))
            {
                if (nodesLeft.contains(*neighbour))
                {
                    open.insert(*neighbour);
                }
            }
        }
        ++i;
    }
    numAreas = i;
}

bool Map::isTaskValid(Node *start, Node *end)
{
    if (!initialPos.contains(start) && !endPos.contains(end))
        return areas[getNode(start)] == areas[getNode(end)] && start->getType() != Node::SpaceType::Wall && *start != *end;
}

std::pair<Node *, Node *> Map::getValidTask()
{
    std::uniform_int_distribution randomX{0, cols - 1};
    std::uniform_int_distribution randomY{0, rows - 1};
    while (true)
    {
        int xStart = randomX(mersenne);
        int yStart = randomY(mersenne);
        int xEnd = randomX(mersenne);
        int yEnd = randomY(mersenne);
        Node *start{getNode(xStart, yStart)};
        Node *end{getNode(xEnd, yEnd)};
        if (isTaskValid(start, end))
        {
            initialPos.insert(start);
            endPos.insert(end);
            return std::pair<Node *, Node *>{start, end};
        }
    }
}

std::map<int, std::pair<Node *, Node *>> Map::getValidTasks(int n)
{
    // std::map<int, std::pair<Node *, Node *>> temp;
    tasks.clear();
    initialPos.clear();
    endPos.clear();
    for (int i{0}; i < n; ++i)
    {
        tasks[i] = getValidTask();
    }
    return tasks;
}

void Map::newMap()
{
    for (Node *n : nodes)
    {
        n->makeFreeSpace();
    }
    setWalls(wallDensity);
    findNeighbours();
    initializeAreas();
}