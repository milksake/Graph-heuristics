#include "CSearch.h"
#include <queue>

CSearch::CSearch(CMatrix* matrix):
    matrix(matrix), state(0)
{}

CSearchDFS::CSearchDFS(const CMatrix::Node& a, const CMatrix::Node b, CMatrix* matrix):
    CSearch(matrix)
{
    begin(a, b);
}

std::vector<CMatrix::Node>& CSearchDFS::nFound()
{
	return DFS_path;
}

std::vector<CMatrix::Node>& CSearchDFS::nDisplay()
{
	return empty;
}

void CSearchDFS::update()
{
    if (state != 0)
        return;
    CMatrix::Node& endNode = *(DFS_path.end() - 1);
    int siblings = matrix->checkSiblings(endNode);
    CMatrix::Node coor[8] = {
        CMatrix::Node(endNode.x - 1, endNode.y - 1),
        CMatrix::Node(endNode.x,     endNode.y - 1),
        CMatrix::Node(endNode.x + 1, endNode.y - 1),
        CMatrix::Node(endNode.x - 1, endNode.y),
        CMatrix::Node(endNode.x + 1, endNode.y),
        CMatrix::Node(endNode.x - 1, endNode.y + 1),
        CMatrix::Node(endNode.x,     endNode.y + 1),
        CMatrix::Node(endNode.x + 1, endNode.y + 1),
    };
    int i = endNode.state;
    siblings = siblings >> i;
    for (; (!(siblings & 0x1) || (std::find(DFS_path.begin(), DFS_path.end(), coor[i]) != DFS_path.end()) || (std::find(DFS_evaluated.begin(), DFS_evaluated.end(), coor[i]) != DFS_evaluated.end())) && siblings; siblings = siblings >> 1, i++);
    if (siblings)
    {
        if (coor[i] == target)
            end(true);
        endNode.state = i;
        DFS_path.push_back(coor[i]);
        DFS_evaluated.push_back(coor[i]);
    }
    else
    {
        CMatrix::Node tmpNode = DFS_path.back();
        DFS_path.pop_back();
        if (DFS_path.empty())
        {
            DFS_path.push_back(tmpNode);
            end(false);
        }
        else
            (DFS_path.end() - 1)->state++;
    }
}

void CSearchDFS::begin(const CMatrix::Node& a, const CMatrix::Node& b)
{
    target = b;
    DFS_path.push_back(a);
}

void CSearchDFS::end(bool s)
{
    state = 2 - s;
}

CSearchBFS::CSearchBFS(const CMatrix::Node& a, const CMatrix::Node b, CMatrix* matrix):
    CSearch(matrix)
{
    begin(a, b);
}

std::vector<CMatrix::Node>& CSearchBFS::nFound()
{
	return BFS_path;
}

std::vector<CMatrix::Node>& CSearchBFS::nDisplay()
{
	return BFS_evaluated;
}

void CSearchBFS::update()
{
    if (state != 0)
        return;
    CMatrix::Node& frontNode = *(BFS_queue.end() - 1);
    BFS_evaluated.push_back(frontNode);
    if (frontNode == target)
    {
        end(true);
        return;
    }
    int siblings = matrix->checkSiblings(frontNode);
    CMatrix::Node coor[8] = {
        CMatrix::Node(frontNode.x - 1, frontNode.y - 1),
        CMatrix::Node(frontNode.x,     frontNode.y - 1),
        CMatrix::Node(frontNode.x + 1, frontNode.y - 1),
        CMatrix::Node(frontNode.x - 1, frontNode.y),
        CMatrix::Node(frontNode.x + 1, frontNode.y),
        CMatrix::Node(frontNode.x - 1, frontNode.y + 1),
        CMatrix::Node(frontNode.x,     frontNode.y + 1),
        CMatrix::Node(frontNode.x + 1, frontNode.y + 1),
    };
    for (int i = 0; siblings; siblings = siblings >> 1, i++)
    {
        if ((siblings & 0x1) && (std::find(BFS_queue.begin(), BFS_queue.end(), coor[i]) == BFS_queue.end()) && (std::find(BFS_evaluated.begin(), BFS_evaluated.end(), coor[i]) == BFS_evaluated.end()))
        {
            BFS_queue.push_front(CMatrix::Node(coor[i], frontNode.y * matrix->width + frontNode.x));
        }
    }
    BFS_queue.pop_back();
    if (BFS_queue.empty())
        end(false);
}

void CSearchBFS::begin(const CMatrix::Node& a, const CMatrix::Node& b)
{
    target = b;
    BFS_queue.push_back(CMatrix::Node(a, -1));
}

void CSearchBFS::end(bool s)
{
    if (s)
    {
        auto it = std::find(BFS_evaluated.begin(), BFS_evaluated.end(), target);
        for (; it != BFS_evaluated.end(); it = std::find(BFS_evaluated.begin(), BFS_evaluated.end(), CMatrix::Node((*it).state, *matrix)))
            BFS_path.push_back((*it));
    }
    state = 2 - s;
}

float CSearchBestFirstS::heuristic(CMatrix::Node x)
{
    return matrix->eucliDistance(x, target);
}

bool CSearchBestFirstS::searchMap(CMatrix::Node n)
{
    for (auto it = open.begin(); it != open.end(); ++it)
        if (it->second == n)
            return true;
    return false;
}

CSearchBestFirstS::CSearchBestFirstS(const CMatrix::Node& a, const CMatrix::Node b, CMatrix* matrix):
    CSearch(matrix)
{
    begin(a, b);
}

std::vector<CMatrix::Node>& CSearchBestFirstS::nFound()
{
    return path;
}

std::vector<CMatrix::Node>& CSearchBestFirstS::nDisplay()
{
    return closed;
}

void CSearchBestFirstS::update()
{
    if (state != 0)
        return;
    auto frontNodeIt = open.begin();
    closed.push_back(frontNodeIt->second);
    if (frontNodeIt->second == target)
    {
        end(true);
        return;
    }
    int siblings = matrix->checkSiblings(frontNodeIt->second);
    CMatrix::Node coor[8] = {
        CMatrix::Node(frontNodeIt->second.x - 1, frontNodeIt->second.y - 1),
        CMatrix::Node(frontNodeIt->second.x,     frontNodeIt->second.y - 1),
        CMatrix::Node(frontNodeIt->second.x + 1, frontNodeIt->second.y - 1),
        CMatrix::Node(frontNodeIt->second.x - 1, frontNodeIt->second.y),
        CMatrix::Node(frontNodeIt->second.x + 1, frontNodeIt->second.y),
        CMatrix::Node(frontNodeIt->second.x - 1, frontNodeIt->second.y + 1),
        CMatrix::Node(frontNodeIt->second.x,     frontNodeIt->second.y + 1),
        CMatrix::Node(frontNodeIt->second.x + 1, frontNodeIt->second.y + 1),
    };
    for (int i = 0; siblings; siblings = siblings >> 1, i++)
    {
        if ((siblings & 0x1) && (!searchMap(coor[i])) && (std::find(closed.begin(), closed.end(), coor[i]) == closed.end()))    
        {
            auto n = CMatrix::Node(coor[i], frontNodeIt->second.y * matrix->width + frontNodeIt->second.x);
            open.emplace(heuristic(n), n);
        }
    }
    open.erase(frontNodeIt);
    if (open.empty())
        end(false);
}

void CSearchBestFirstS::begin(const CMatrix::Node& a, const CMatrix::Node& b)
{
    target = b;
    open.emplace(heuristic(a), CMatrix::Node(a, -1));
}

void CSearchBestFirstS::end(bool s)
{
    if (s)
    {
        auto it = std::find(closed.begin(), closed.end(), target);
        for (; it != closed.end(); it = std::find(closed.begin(), closed.end(), CMatrix::Node((*it).state, *matrix)))
            path.push_back((*it));
    }
    state = 2 - s;
}

///////////////////////////////////////////////////////////////////////////////////////

float CSearchAStar::heuristic(CMatrix::Node x)
{
    return matrix->eucliDistance(x, target);
}

bool CSearchAStar::searchMap(CMatrix::Node n)
{
    for (auto it = open.begin(); it != open.end(); ++it)
        if (it->second == n)
            return true;
    return false;
}

CSearchAStar::CSearchAStar(const CMatrix::Node& a, const CMatrix::Node b, CMatrix* matrix) :
    CSearch(matrix)
{
    begin(a, b);
}

std::vector<CMatrix::Node>& CSearchAStar::nFound()
{
    return path;
}

std::vector<CMatrix::Node>& CSearchAStar::nDisplay()
{
    return closed;
}

void CSearchAStar::update()
{
    if (state != 0)
        return;
    auto frontNodeIt = open.begin();
    closed.push_back(frontNodeIt->second);
    if (frontNodeIt->second == target)
    {
        end(true);
        return;
    }
    int siblings = matrix->checkSiblings(frontNodeIt->second);
    CMatrix::Node coor[8] = {
        CMatrix::Node(frontNodeIt->second.x - 1, frontNodeIt->second.y - 1),
        CMatrix::Node(frontNodeIt->second.x,     frontNodeIt->second.y - 1),
        CMatrix::Node(frontNodeIt->second.x + 1, frontNodeIt->second.y - 1),
        CMatrix::Node(frontNodeIt->second.x - 1, frontNodeIt->second.y),
        CMatrix::Node(frontNodeIt->second.x + 1, frontNodeIt->second.y),
        CMatrix::Node(frontNodeIt->second.x - 1, frontNodeIt->second.y + 1),
        CMatrix::Node(frontNodeIt->second.x,     frontNodeIt->second.y + 1),
        CMatrix::Node(frontNodeIt->second.x + 1, frontNodeIt->second.y + 1),
    };
    for (int i = 0; siblings; siblings = siblings >> 1, i++)
    {
        if ((siblings & 0x1) && (!searchMap(coor[i])) && (std::find(closed.begin(), closed.end(), coor[i]) == closed.end()))
        {
            auto n = CMatrix::Node(coor[i], frontNodeIt->second.y * matrix->width + frontNodeIt->second.x);
            int g = matrix->eucliDistance(frontNodeIt->second, n);
            int h = heuristic(n);
            int f = g + h;
            open.emplace(f, n);
        }
    }
    open.erase(frontNodeIt);
    if (open.empty())
        end(false);
}

void CSearchAStar::begin(const CMatrix::Node& a, const CMatrix::Node& b)
{
    target = b;
    open.emplace(heuristic(a), CMatrix::Node(a, -1));
}

void CSearchAStar::end(bool s)
{
    if (s)
    {
        auto it = std::find(closed.begin(), closed.end(), target);
        for (; it != closed.end(); it = std::find(closed.begin(), closed.end(), CMatrix::Node((*it).state, *matrix)))
            path.push_back((*it));
    }
    state = 2 - s;
}
