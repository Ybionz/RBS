#include "map.h"

Map::Map(int a, int b, double wallDensity)
    : cols{a},
      rows{b},
      mersenne{static_cast<std::mt19937::result_type>(std::time(nullptr))}
{
    initializeNodes(rows, cols);
    setWalls(wallDensity);
    findNeighbours();
    initializeAllActions();
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

bool Map::printMap()
{
    for (int i{0}; i < rows; ++i)
    {
        for (int j{0}; j < cols; ++j)
        {
            if (getNode(j, i)->getType() == Node::SpaceType::Wall)
            {
                std::cout << "+";
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

std::set<Node *> Map::_getNeighbours(Node *n)
{
    std::set<Node *> neighbours{};
    int x{n->getX()};
    int y{n->getY()};
    if (isInMap(x - 1, y))
    {
        neighbours.insert(getNode(x - 1, y));
    }
    if (isInMap(x, y - 1))
    {
        neighbours.insert(getNode(x, y - 1));
    }
    if (isInMap(x + 1, y))
    {
        neighbours.insert(getNode(x + 1, y));
    }
    if (isInMap(x, y + 1))
    {
        neighbours.insert(getNode(x, y + 1));
    }

    return neighbours;
};

std::set<Node *> Map::getNeighbours(Node *n)
{
    return neighbours[n];
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