#include "COpenGL.h"
#include <iostream>

COpenGL::COpenGL()
{

}

bool COpenGL::init(int windowW, int windowH)
{
    /* Initialize the library */
    if (!glfwInit())
        return false;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowW, windowH, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Extra configurations*/
    glClearColor(0, 0, 0, 1);

    return true;
}

void COpenGL::run(const CMatrix& matrix)
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        draw(matrix);
    }
    glfwTerminate();
}

void COpenGL::draw(const CMatrix& matrix)
{
    /* Calculate space */
    float sepX = 2.0f / (float)(matrix.width + 1);
    float sepY = 2.0f / (float)(matrix.height + 1);

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* DRAW GRID */
    glBegin(GL_LINES);
        //glColor3f(1, 1, 1);
        for (int x = 0; x < matrix.width; x++)
        {
            for (int y = 0; y < matrix.height; y++)
            {
                if (matrix.getNode(x, y))
                {
                    //glColor3f((1.0f * (float)x)/(float)(matrix.width), (1.0f * (float)y) / (float)(matrix.height), 1);
                    bool checkS[8];
                    matrix.checkSiblings(x, y, checkS);
                    int siblingsX[8] = { x - 1, x, x + 1, x - 1, x + 1, x - 1, x, x + 1 };
                    int siblingsY[8] = { y - 1, y - 1, y - 1, y, y, y + 1, y + 1, y + 1 };
                    for (int i = 0; i < 8; i++)
                    {
                        if (checkS[i])
                        {
                            glVertex2f(sepX * (x + 1) - 1, sepY * (y + 1) - 1);
                            glVertex2f(sepX * (siblingsX[i] + 1) - 1, sepY * (siblingsY[i] + 1) - 1);
                        }
                    }
                }
            }
        }
    glEnd();

    glBegin(GL_QUADS);
        //glColor3f(255.0f / 255.0f, 219.0f / 255.0f, 88.0f / 255.0f);
        for (int x = 0; x < matrix.width; x++)
        {
            for (int y = 0; y < matrix.height; y++)
            {
                if (matrix.getNode(x, y))
                {
                    float coorX = sepX * (x + 1) - 1;
                    float coorY = sepY * (y + 1) - 1;
                    glVertex2f(coorX - sepX / 10.0f, coorY);
                    glVertex2f(coorX, coorY - sepY / 10.0f);
                    glVertex2f(coorX + sepX / 10.0f, coorY);
                    glVertex2f(coorX, coorY + sepY / 10.0f);
                }
            }
        }
    glEnd();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}
