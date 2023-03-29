#include "COpenGL.h"
#include <iostream>
#include "CInput.h"

COpenGL::COpenGL(int frameT):
    frame_time(frameT), current_frame(0), frame_count(frameT)
{}

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

void COpenGL::run(CMatrix& matrix)
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* User input */
        if (matrix.state != 1 && matrix.state != 2)
            CInput::update(this, &matrix);
        if (frame_count == frame_time)
        {
            frame_count = 0;
            current_frame++;
            matrix.update();
        }
        draw(matrix);
        frame_count++;
    }
    glfwTerminate();
}

void COpenGL::draw(const CMatrix& matrix)
{
    /* Calculate space */
    float sepX = 2.0f / (float)(matrix.width + 1);
    float sepY = 2.0f / (float)(matrix.height + 1);

    float cR = (matrix.state == 4) * 1;
    float cG = (matrix.state == 3) * 1;
    float cB = (matrix.state == 1) + (matrix.state == 2);

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* DRAW GRID */
    glBegin(GL_LINES);
        //glColor3f(1.0f /(rand() % 10), 1.0f / (rand() % 10), 1.0f / (rand() % 10));
        for (int x = 0; x < matrix.width; x++)
        {
            for (int y = 0; y < matrix.height; y++)
            {
                if (matrix.getNode(x, y))
                {
                    //glColor3f((1.0f * (float)x)/(float)(matrix.width), (1.0f * (float)y) / (float)(matrix.height), 1);
                    int checkS = matrix.checkSomeSiblings(x, y);
                    int siblingsX[4] = { x + 1, x, x + 1, x + 1 };
                    int siblingsY[4] = { y, y + 1, y + 1, y - 1 };
                    for (int i = 0; i < 4; i++, checkS = checkS >> 1)
                    {
                        if (checkS & 0x1)
                        {
                            glVertex2f(sepX * (x + 1) - 1, sepY * (y + 1) - 1);
                            glVertex2f(sepX * (siblingsX[i] + 1) - 1, sepY * (siblingsY[i] + 1) - 1);
                        }
                    }
                }
            }
        }
    glEnd();

    if (matrix.BFS_evaluated.size() > 1)
    {
        glBegin(GL_LINES);
            for (int i = 0; i < matrix.BFS_evaluated.size(); i++)
            {
                if (matrix.BFS_evaluated[i].state != -1)
                {
                    int x, y;
                    matrix.getCoord(matrix.BFS_evaluated[i].state, x, y);
                    glColor3f(cR, 0, !cR);
                    glVertex2f(sepX * (x + 1) - 1, sepY * (y + 1) - 1);
                    glVertex2f(sepX * (matrix.BFS_evaluated[i].x + 1) - 1, sepY * (matrix.BFS_evaluated[i].y + 1) - 1);
                }
            }
        glEnd();
    }

    if (matrix.DFS_path.size() > 1)
    {
        glBegin(GL_LINE_STRIP);
        glColor3f(cR, cG, cB);
        for (int i = 0; i < matrix.DFS_path.size(); i++)
            glVertex2f(sepX * (matrix.DFS_path[i].x + 1) - 1, sepY * (matrix.DFS_path[i].y + 1) - 1);
        glEnd();
    }

    glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        for (int x = 0; x < matrix.width; x++)
        {
            for (int y = 0; y < matrix.height; y++)
            {
                if (matrix.getNode(x, y))
                {
                    float coorX = sepX * (x + 1) - 1;
                    float coorY = sepY * (y + 1) - 1;
                    if (std::find(matrix.selected_nodes.begin(), matrix.selected_nodes.end(), CMatrix::Node(x, y)) != matrix.selected_nodes.end())
                        glColor3f(1, 0, 1);
                    if (std::find(matrix.BFS_evaluated.begin(), matrix.BFS_evaluated.end(), CMatrix::Node(x, y)) != matrix.BFS_evaluated.end())
                        glColor3f(cR, 0, !cR);
                    if (std::find(matrix.DFS_path.begin(), matrix.DFS_path.end(), CMatrix::Node(x, y)) != matrix.DFS_path.end())
                        glColor3f(cR, cG, cB);
                    if (CMatrix::Node(x, y) == matrix.target)
                        glColor3f(cR, cB || cG, 0);
                    glVertex2f(coorX - sepX / 10.0f, coorY);
                    glVertex2f(coorX, coorY - sepY / 10.0f);
                    glVertex2f(coorX + sepX / 10.0f, coorY);
                    glVertex2f(coorX, coorY + sepY / 10.0f);
                    glColor3f(1, 1, 1);
                }
            }
        }
    glEnd();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}
