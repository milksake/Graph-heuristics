#include "CMatrix.h"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "CInput.h"
#include "CSearch.h"

CMatrix::CMatrix(int width, int height):
    width(width), height(height), obstacle(-1, -1, -1)
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

bool CMatrix::checkRange(Node lim1, Node lim2, Node value) const
{
    return std::min(lim1.x, lim2.x) <= value.x
        && value.x <= std::max(lim1.x, lim2.x)
        && std::min(lim1.y, lim2.y) <= value.y
        && value.y <= std::max(lim1.y, lim2.y);
}

void CMatrix::update()
{
    if (search)
        search->update();
}

void CMatrix::resize(int percent)
{
    /* Fix bugs */
    delete search;
    search = nullptr;
    selected_nodes.clear();
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
void CMatrix::eraseRange(Node a, Node b)
{
    /* Fix bugs */
    delete search;
    search = nullptr;
    selected_nodes.clear();
    /* Modify matrix */
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (checkRange(a, b, CMatrix::Node(x, y)))
                setNode(x, y, false);
        }
    }
}
/*
template<typename T>
void CMatrix::beginSearch(const Node& a, const Node& b)
{
    selected_nodes.clear();
    if (search)
        delete search;
    search = new T(a, b, this);
}
*/