#include "map.h"

Map::Map(int a, int b, double wallDensity)
    : cols{a},
      rows{b},
      mersenne{static_cast<std::mt19937::result_type>(std::time(nullptr))}
{
    initializeNodes(rows, cols);
    setWalls(wallDensity);
    initializeAllActions();
    findNeighbours();
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
    auto it = path.begin();
    it++; // because first action is wait
    Node *current{getNode(initial.getX(), initial.getY())};

    for (int i{0}; i < rows; ++i)
    {
        for (int j{0}; j < cols; ++j)
        {
            if (getNode(j, i)->getType() == Node::SpaceType::Wall)
            {
                std::cout << char(0xdb);
            }
            else if (path.size() > 0 && *getNode(j, i) == *current)
            {
                std::cout << "x";
                current = current + *it;
                it++;
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
        if (isInMap(n + a))
        {
            neighbours.insert(std::pair<Action, Node *>{a, n + a});
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
    for (int i = Action::Direction::north; i != Action::Direction::Last; ++i)
    {
        allActions.insert(Action(static_cast<Action::Direction>(i)));
    }
};

