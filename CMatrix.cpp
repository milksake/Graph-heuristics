#include "CMatrix.h"
#include <cstdlib>
#include <time.h>

CMatrix::CMatrix(int width, int height):
    width(width), height(height)
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
}

int CMatrix::getNode(int x, int y) const
{
    return matrix[y * width + x];
}

void CMatrix::checkSiblings(int x, int y, bool* check) const
{
    check[0] = checkNode(x - 1, y - 1); check[1] = checkNode(x, y - 1); check[2] = checkNode(x + 1, y - 1);
    check[3] = checkNode(x - 1, y);                                     check[4] = checkNode(x + 1, y);
    check[5] = checkNode(x - 1, y + 1); check[6] = checkNode(x, y + 1); check[7] = checkNode(x + 1, y + 1);
}

bool CMatrix::checkNode(int x, int y) const
{
    return (0 <= x) && (x < width) && (0 <= y) && (y < height) && (getNode(x, y));
}
