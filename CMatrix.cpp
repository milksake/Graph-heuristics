#include "CMatrix.h"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "CInput.h"

CMatrix::CMatrix(int width, int height):
    width(width), height(height), state(0)
{
    init();
}

CMatrix::~CMatrix()
{
    /* Cleaning */
    delete[] matrix;
    matrix = nullptr;
}

void CMatrix::init()
{
    srand(time(0));

    /* Create matrix */
    matrix = new bool[width * height];
    for (int i = 0; i < width * height; i++)
    {
        matrix[i] = true;
    }
}

bool CMatrix::getNode(int x, int y) const
{
    return matrix[y * width + x];
}

int CMatrix::checkSiblings(int x, int y) const
{
    int check = 0;
    check = (int)checkNode(x - 1, y - 1);
    check += (int)checkNode(x, y - 1) << 1;
    check += (int)checkNode(x + 1, y - 1) << 2;
    check += (int)checkNode(x - 1, y) << 3;
    check += (int)checkNode(x + 1, y) << 4;
    check += (int)checkNode(x - 1, y + 1) << 5;
    check += (int)checkNode(x, y + 1) << 6;
    check += (int)checkNode(x + 1, y + 1) << 7;
    return check;
}

int CMatrix::checkSiblings(Node n) const
{
    return checkSiblings(n.x, n.y);
}

int CMatrix::checkSomeSiblings(int x, int y) const
{
    int check = 0;
    check = (int)checkNode(x + 1, y);
    check += (int)checkNode(x, y + 1) << 1;
    check += (int)checkNode(x + 1, y + 1) << 2;
    check += (int)checkNode(x + 1, y - 1) << 3;
    return check;
}

void CMatrix::setNode(int x, int y, bool set)
{
    matrix[y * width + x] = set;
}

void CMatrix::getCoord(int index, int& x, int& y) const
{
    x = index % width;
    y = index / width;
}

bool CMatrix::checkNode(int x, int y) const
{
    return (0 <= x) && (x < width) && (0 <= y) && (y < height) && (getNode(x, y));
}

bool CMatrix::checkNode(Node n) const
{
    return checkNode(n.x, n.y);
}

void CMatrix::update()
{
    switch (state)
    {
    case 1:
        DFS();
        break;
    case 2:
        BFS();
        break;
    default:
        break;
    }
}

void CMatrix::beginDFS(const Node& a, const Node& b)
{
    target = b;
    state = 1;
    selected_nodes.clear();
    DFS_path.clear();
    DFS_evaluated.clear();
    BFS_queue.clear();
    BFS_evaluated.clear();
    DFS_path.push_back(a);
}

void CMatrix::beginBFS(const Node& a, const Node& b)
{
    target = b;
    state = 2;
    selected_nodes.clear();
    DFS_path.clear();
    DFS_evaluated.clear();
    BFS_queue.clear();
    BFS_evaluated.clear();
    BFS_queue.push_back(Node(a, -1));
}

void CMatrix::DFS()
{
    Node& endNode = *(DFS_path.end() - 1);
    int siblings = checkSiblings(endNode);
    Node coor[8] = {
        Node(endNode.x - 1, endNode.y - 1),
        Node(endNode.x,     endNode.y - 1),
        Node(endNode.x + 1, endNode.y - 1),
        Node(endNode.x - 1, endNode.y),
        Node(endNode.x + 1, endNode.y),
        Node(endNode.x - 1, endNode.y + 1),
        Node(endNode.x,     endNode.y + 1),
        Node(endNode.x + 1, endNode.y + 1),
    };
    int i = endNode.state;
    siblings = siblings >> i;
    for (; (!(siblings & 0x1) || (std::find(DFS_path.begin(), DFS_path.end(), coor[i]) != DFS_path.end()) || (std::find(DFS_evaluated.begin(), DFS_evaluated.end(), coor[i]) != DFS_evaluated.end())) && siblings; siblings = siblings >> 1, i++);
    if (siblings)
    {
        if (coor[i] == target)
            endDFS(true);
        endNode.state = i;
        DFS_path.push_back(coor[i]);
        DFS_evaluated.push_back(coor[i]);
    }
    else
    {
        Node tmpNode = DFS_path.back();
        DFS_path.pop_back();
        if (DFS_path.empty())
        {
            DFS_path.push_back(tmpNode);
            endDFS(false);
        }
        else
            (DFS_path.end() - 1)->state++;
    }
}

void CMatrix::BFS()
{
    Node& frontNode = *(BFS_queue.end() - 1);
    BFS_evaluated.push_back(frontNode);
    if (frontNode == target)
    {
        endBFS(true);
        return;
    }
    int siblings = checkSiblings(frontNode);
    Node coor[8] = {
        Node(frontNode.x - 1, frontNode.y - 1),
        Node(frontNode.x,     frontNode.y - 1),
        Node(frontNode.x + 1, frontNode.y - 1),
        Node(frontNode.x - 1, frontNode.y),
        Node(frontNode.x + 1, frontNode.y),
        Node(frontNode.x - 1, frontNode.y + 1),
        Node(frontNode.x,     frontNode.y + 1),
        Node(frontNode.x + 1, frontNode.y + 1),
    };
    for (int i = 0; siblings; siblings = siblings >> 1, i++)
    {
        if ((siblings & 0x1) && (std::find(BFS_queue.begin(), BFS_queue.end(), coor[i]) == BFS_queue.end()) && (std::find(BFS_evaluated.begin(), BFS_evaluated.end(), coor[i]) == BFS_evaluated.end()))
        {
            BFS_queue.push_front(Node(coor[i], frontNode.y * width + frontNode.x));
        }
    }
    //std::cout << "Processed: " << frontNode.x << ' ' << frontNode.y << '\n';
    BFS_queue.pop_back();
    if (BFS_queue.empty())
        endBFS(false);
}

void CMatrix::endDFS(bool s)
{
    state = 4 - s;
}

void CMatrix::resize(int percent)
{
    /* Fix bugs */
    DFS_path.clear();
    DFS_evaluated.clear();
    BFS_queue.clear();
    BFS_evaluated.clear();
    selected_nodes.clear();
    state = 0;
    /* Modify matrix */
    for (int i = 0; i < width * height; i++)
    {
        matrix[i] = true;
    }
    for (int numNodes = width * height * (100 - percent) / 100; numNodes > 0; numNodes--)
    {
        matrix[rand() % (width * height)] = false;
    }
}

void CMatrix::endBFS(bool s)
{
    state = 4 - s;
    if (s)
    {    
        auto it = std::find(BFS_evaluated.begin(), BFS_evaluated.end(), target);
        for (; it != BFS_evaluated.end(); it = std::find(BFS_evaluated.begin(), BFS_evaluated.end(), Node((*it).state, *this)))
            DFS_path.push_back((*it));
    }
}
