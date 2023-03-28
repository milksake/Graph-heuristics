#include "COpenGL.h"
#include "CMatrix.h"

int main(void)
{
    COpenGL openGL;
    CMatrix matrix(30, 20);

    if (!openGL.init(1200, 670))
        return -1;
    openGL.run(matrix);

    return 0;
}