#include "map.h"

Map::Map(int a, int b, double wallDensity, int _agents)
    : cols{a},
      rows{b},
      wallDensity{wallDensity},
      agents{_agents},
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

std::map<Action, Node *> Map::_getNeighbours(Node *n)
{
    std::map<Action, Node *> neighbours{};
    int x{n->getX()};
    int y{n->getY()};
    for (Action a : allActions)
    {
        Node *neigh = n + a;
        if (isInMap(neigh) && getNode(neigh)->getType() != Node::SpaceType::Wall)
        {
            neighbours[a] = neigh;
            // neighbours.insert(std::pair<Action, Node *>{a, neigh});
        }
    }

    return neighbours;
};

std::map<Action, Node *> Map::getNeighbours(Node *n)
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

std::pair<double, Node> Map::distTask(Node n, subtask_t subtask)
{
    LightState best;
    double dist{static_cast<double>(rows * cols)};
    for (auto l : subtask)
    {
        double newDist{distNode(n, *l.getMove().getPrev())};
        if (newDist < dist)
        {
            dist = newDist;
            best = l;
        }
    }
    return std::pair<double, Node>{dist, *best.getCurrent()};
}

double Map::dist(Node start, Node goal, subtasks_t subtasks, int label)
{
    double dist{0};
    Node current{start};
    for (int i{label}; i < subtasks.size(); i++)
    {
        const subtask_t  sub{subtasks[i]};
        auto [newDist, nextNode] = distTask(current, sub);
        dist += newDist + 1; // because the move takes a turn
        current = nextNode;
    }
    dist += distNode(current, goal);
    return dist;
};

double Map::distNode(Node n1, Node n2)
{
    int x{abs(n1.getX() - n2.getX())};
    int y{abs(n1.getY() - n2.getY())};
    return std::sqrt(x * x + y * y);
};

int Map::distNodeMoves(Node n1, Node n2)
{
    int x{abs(n1.getX() - n2.getX())};
    int y{abs(n1.getY() - n2.getY())};
    return x + y;
};

int Map::distMoves(Node n1, Node n2, subtasks_t subtasks)
{
    int x{abs(n1.getX() - n2.getX())};
    int y{abs(n1.getY() - n2.getY())};
    return x + y;
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
    };
    numAreas = i;
}

bool Map::isMissionValid(Node *start, Node *end)
{
    if (!initialPos.contains(start) && !endPos.contains(end))
        return areas[getNode(start)] == areas[getNode(end)] && start->getType() != Node::SpaceType::Wall && *start != *end;
    return false;
}

std::pair<Node *, Node *> Map::getValidMission()
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
        if (isMissionValid(start, end))
        {
            initialPos.insert(start);
            endPos.insert(end);
            return std::pair<Node *, Node *>{start, end};
        }
    }
}

missions_t Map::getValidMissions(int n)
{
    // std::map<int, std::pair<Node *, Node *>> temp;
    bool valid{false};
    while (!valid)
    {
        missions.clear();
        initialPos.clear();
        endPos.clear();
        for (int i{0}; i < n; ++i)
        {
            missions[i] = getValidMission();
        }
        valid = doesMissionsFollowRules();
    }
    return missions;
};

bool Map::doesMissionsFollowRules()
{
    // Agents can not share intial or end pos at this point.
    // This check does not guarantee that the level can be completed under the given rules
    // But if the check fails then the level is not completable under the given rules.
    std::set<LightState> initialStates;
    std::set<LightState> endStates;
    for (auto node : initialPos)
    {
        initialStates.insert(LightState(node, Action::Direction::wait));
    }
    for (auto node : endPos)
    {
        endStates.insert(LightState(node, Action::Direction::wait));
    }
    for (auto state : initialStates)
    {
        for (auto rule : rulesRestrict)
        {
            auto states = rule.getRestrict(state);
            std::vector<LightState> inter;
            std::set_intersection(initialStates.begin(), initialStates.end(), states.begin(), states.end(), std::back_inserter(inter));
            if (inter.size() > rule.count)
            {
                std::cout << "Initial pos did not follow rules\n";
                return false;
            }
        }
    };
    for (auto state : endStates)
    {
        for (auto rule : rulesRestrict)
        {
            auto states = rule.getRestrict(state);
            std::vector<LightState> inter;
            std::set_intersection(endStates.begin(), endStates.end(), states.begin(), states.end(), std::back_inserter(inter));
            if (inter.size() > rule.count)
            {
                std::cout << "End pos did not follow rules\n";
                return false;
            }
        }
    }
    return true;
};

missions_t Map::getMissions(std::set<agentID_t> agentSet)
{
    missions_t temp;
    for (auto agent : agentSet)
    {
        temp[agent] = missions[agent];
    }
    return temp;
}

missions_t Map::getMissions(agentID_t lastAgent)
{
    std::set<agentID_t> tempAgents;
    for (int agent{0}; agent <= lastAgent; agent++)
    {
        tempAgents.insert(agent);
    }
    return getMissions(tempAgents);
}

void Map::setMissions(missions_t _missions)
{
    missions = _missions;
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
};

void Map::setRules(std::set<RuleRestrict> _rulesRestrict, std::set<RuleRequest> _rulesRequest)
{
    rulesRestrict = _rulesRestrict;
    rulesRequest = _rulesRequest;
    missions = getValidMissions(agents);
}

void Map::saveLevel(std::string name, paths_t paths)
{
    int i{0};
    std::string file{name + std::to_string(i)};
    fs::path path = fs::current_path() / "levels" / file;
    while (fs::is_directory(path))
    {
        i++;
        file = name + std::to_string(i);
        path = fs::current_path() / "levels" / file;
    }
    // fs::path path{fs::current_path() / "levels" / name / };
    int status = fs::create_directories(path);
    std::cout << "dir was created " << status << '\n';
    fs::path scenPath{path / "level.scen"};
    fs::path mapPath{path / "level.map"};
    std::ofstream scenFile(scenPath.string());
    std::ofstream mapFile(mapPath.string());
    std::cout << "scen is open " << scenFile.is_open() << '\n';
    std::cout << "map is open " << mapFile.is_open() << '\n';

    scenFile << "Version 1\n";
    for (int i{0}; i < agents; i++)
    {
        Node *start{missions[i].first};
        Node *end{missions[i].second};
        scenFile << "XX\tXX\tXX\tXX\t";
        scenFile << start->getX();
        scenFile << '\t';
        scenFile << start->getY();
        scenFile << '\t';
        scenFile << end->getX();
        scenFile << '\t';
        scenFile << end->getY();
        scenFile << "\tXX \n";
    }
    scenFile.close();

    mapFile << "type octile\n";
    mapFile << "height " + std::to_string(rows) + '\n';
    mapFile << "width " + std::to_string(cols) + '\n';
    mapFile << "map\n";
    // mapFile <<  + '\n';
    // mapFile << '\n';
    // mapFile << "width" + cols + '\n';
    // mapFile << "map\n";

    for (int row{0}; row < rows; row++)
    {
        for (int col{0}; col < cols; col++)
        {
            Node *node{getNode(col, row)};
            mapFile << ((node->getType() == Node::SpaceType::Wall) ? "+" : ".");
        }
        mapFile << '\n';
    }
    mapFile.close();

    if (paths.size() < 1)
        return;

    fs::path solPath{path / "CBS.txt"};
    std::ofstream solFile(solPath.string());

    int maxLen{0};
    std::map<agentID_t, path_t::iterator> iters;
    for (int i{0}; i < agents; i++)
    {
        maxLen = std::max(static_cast<int>(paths[i].size()), maxLen);
        iters[i] = paths[i].begin();
    }
    for (int time{0}; time < maxLen; time++)
    {
        for (int agent{0}; agent < agents; agent++)
        {
            if (iters[agent] == paths[agent].end())
            {
                solFile << "WAIT";
            }
            else
            {
                auto state = *(iters[agent]++);
                auto action = state->action;
                solFile << action;
            }
            if (!(agent == agents - 1))
            {
                solFile << ",";
            }
        }
        solFile << '\n';
    }
    solFile.close();
}
