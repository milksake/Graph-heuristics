#include "CMatrix.h"
#include <cstdlib>
#include <time.h>

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
        matrix[i] = rand() % 2;
    }
    // ERASE
    setNode(2, 4, true); setNode(20, 10, true);
    // END ERASE
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

bool CMatrix::checkNode(int x, int y) const
{
    return (0 <= x) && (x < width) && (0 <= y) && (y < height) && (getNode(x, y));
}

void CMatrix::update()
{
    switch (state)
    {
    case 1:
        DFS();
    default:
        break;
    }
}

void CMatrix::beginDFS(const Node& a, const Node& b)
{
    target = b;
    state = 1;
    DFS_path.push_back(a);
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
    for (; (!(siblings & 0x1) || (std::find(DFS_path.begin(), DFS_path.end(), coor[i]) != DFS_path.end())) && siblings; siblings = siblings >> 1, i++);
    if (siblings)
    {
        if (coor[i] == target)
            endDFS(true);
        endNode.state = i;
        DFS_path.push_back(coor[i]);
    }
    else
    {
        DFS_path.pop_back();
        (DFS_path.end() - 1)->state++;
        if (DFS_path.empty())
            endDFS(false);
    }
}

void CMatrix::endDFS(bool s)
{
    state = 4 - s;
}