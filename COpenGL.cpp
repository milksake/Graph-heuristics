#include "COpenGL.h"
#include <iostream>

COpenGL::COpenGL(int frameT):
    frame_time(frameT), current_frame(0), frame_count(frameT)
{
     input.context = this;
}

bool COpenGL::init(int windowW, int windowH)
{
    /* Initialize the library */
    if (!glfwInit())
        return false;

    /* Create a windowed mode window and its OpenGL context */
    wWidth = windowW;
    wHeight = windowH;
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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    CInput* input = static_cast<CInput*>(glfwGetWindowUserPointer(window));
    CMatrix* matrix_ptr = input->matrix;

    if (matrix_ptr == nullptr) return;

    if (key == GLFW_KEY_B && action == GLFW_PRESS && matrix_ptr->selected_nodes.size() == 2)
    {
        matrix_ptr->beginBFS(matrix_ptr->selected_nodes[0], matrix_ptr->selected_nodes[1]);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS && matrix_ptr->selected_nodes.size() == 2)
    {
        matrix_ptr->beginDFS(matrix_ptr->selected_nodes[0], matrix_ptr->selected_nodes[1]);
    }
    if (GLFW_KEY_0 <= key && key <= GLFW_KEY_9 && action == GLFW_PRESS)
    {
        int value = (key - GLFW_KEY_0) * 10;
        matrix_ptr->resize(value ? value : 100);
    }
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    CInput* input = static_cast<CInput*>(glfwGetWindowUserPointer(window));
    CMatrix* matrix_ptr = input->matrix;
    COpenGL* context_ptr = input->context;
    if (matrix_ptr == nullptr || context_ptr == nullptr) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        context_ptr->convertToGLCoor(x, y);
        auto node = context_ptr->getNodePosition(x, y);

        if (matrix_ptr->checkNode(node) && matrix_ptr->selected_nodes.size() < 2)
        {
            matrix_ptr->selected_nodes.push_back(node);
        }
    }
}

void COpenGL::run(CMatrix& matrix)
{
    input.matrix = &matrix;
    glfwSetWindowUserPointer(window, &input);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (frame_count == frame_time)
        {
            // ERASE
            if (current_frame == 7)
            {
                //matrix.beginBFS(CMatrix::Node(2, 4), CMatrix::Node(20, 10));
            }
            // END ERASE
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
                    if (std::find(matrix.BFS_evaluated.begin(), matrix.BFS_evaluated.end(), CMatrix::Node(x, y)) != matrix.BFS_evaluated.end())
                        glColor3f(cR, 0, !cR);
                    if (std::find(matrix.DFS_path.begin(), matrix.DFS_path.end(), CMatrix::Node(x, y)) != matrix.DFS_path.end())
                        glColor3f(cR, cG, cB);
                    if (CMatrix::Node(x, y) == matrix.target)
                        glColor3f(cR, cB || cG, 0);
                    if (std::find(matrix.selected_nodes.begin(), matrix.selected_nodes.end(), CMatrix::Node(x, y)) != matrix.selected_nodes.end())
                        glColor3f(1, 0, 1);
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

void COpenGL::convertToGLCoor(double& x, double& y)
{
    x = (float)(2 * x) / (float)wWidth - 1;
    y = -((float)(2 * y) / (float)wHeight - 1);
}

CMatrix::Node COpenGL::getNodePosition(float x, float y)
{
    /* Calculate space */
    float sepX = 2.0f / (float)(input.matrix->width + 1);
    float sepY = 2.0f / (float)(input.matrix->height + 1);
    return CMatrix::Node((x + 1) / sepX - 0.5f, (y + 1) / sepY - 0.5f);
}
