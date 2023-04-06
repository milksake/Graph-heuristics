#include "COpenGL.h"
#include "CSearch.h"
#include <iostream>

COpenGL::COpenGL(int frameT):
    frame_time(frameT), current_frame(0), frame_count(frameT), obstacle(-1, -1, -1)
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
    COpenGL* context_ptr = input->context;

    if (matrix_ptr == nullptr) return;

    if (key == GLFW_KEY_B && action == GLFW_PRESS && matrix_ptr->selected_nodes.size() == 2)
    {
        matrix_ptr->beginSearch<CSearchBFS>(matrix_ptr->selected_nodes[0], matrix_ptr->selected_nodes[1]);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS && matrix_ptr->selected_nodes.size() == 2)
    {
        matrix_ptr->beginSearch<CSearchDFS>(matrix_ptr->selected_nodes[0], matrix_ptr->selected_nodes[1]);
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS && matrix_ptr->selected_nodes.size() == 2)
    {
        matrix_ptr->beginSearch<CSearchBestFirstS>(matrix_ptr->selected_nodes[0], matrix_ptr->selected_nodes[1]);
    }
    if (GLFW_KEY_0 <= key && key <= GLFW_KEY_9 && action == GLFW_PRESS)
    {
        int value = (key - GLFW_KEY_0) * 10;
        matrix_ptr->resize(value ? value : 100);
        context_ptr->obstacles.clear();
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

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        context_ptr->convertToGLCoor(x, y);
        if (action == GLFW_PRESS)
        {
            context_ptr->obstacle = context_ptr->getNodePosition(x, y);
        }
        if (action == GLFW_RELEASE && context_ptr->obstacle.state != -1)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            context_ptr->convertToGLCoor(x, y);
            auto nodePos = context_ptr->getNodePosition(x, y);
            matrix_ptr->eraseRange(nodePos, context_ptr->obstacle);
            context_ptr->obstacles.push_back(CMatrix::Node(context_ptr->obstacle, nodePos.y * matrix_ptr->width + nodePos.x));

            context_ptr->obstacle.state = -1;
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

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* DRAW GRID */
    glBegin(GL_LINES);
        for (int x = 0; x < matrix.width; x++)
        {
            for (int y = 0; y < matrix.height; y++)
            {
                if (matrix.getNode(x, y))
                {
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

    if (matrix.search && matrix.search->nDisplay().size() > 1)
    {
        glBegin(GL_LINES);
            bool c = matrix.search->state == 2;
            glColor3f(c, 0, !c);
            for (int i = 0; i < matrix.search->nDisplay().size(); i++)
            {
                if (matrix.search->nDisplay()[i].state != -1)
                {
                    int x, y;
                    matrix.getCoord(matrix.search->nDisplay()[i].state, x, y);
                    glVertex2f(sepX * (x + 1) - 1, sepY * (y + 1) - 1);
                    glVertex2f(sepX * (matrix.search->nDisplay()[i].x + 1) - 1, sepY * (matrix.search->nDisplay()[i].y + 1) - 1);
                }
            }
        glEnd();
    }

    if (matrix.search && matrix.search->nFound().size() > 1)
    {
        glBegin(GL_LINE_STRIP);
            glColor3f(matrix.search->state == 2 ? 1 : 0, matrix.search->state == 1 ? 1 : 0, !matrix.search->state);
            for (int i = 0; i < matrix.search->nFound().size(); i++)
                glVertex2f(sepX * (matrix.search->nFound()[i].x + 1) - 1, sepY * (matrix.search->nFound()[i].y + 1) - 1);
        glEnd();
    }

    glBegin(GL_QUADS);
        if (obstacle.state != -1)
        {
            double x_o, y_o;
            glfwGetCursorPos(window, &x_o, &y_o);
            convertToGLCoor(x_o, y_o);
            auto lim = getNodePosition(x_o, y_o);

            float coorX_1 = sepX * (std::min(obstacle.x, lim.x) + 1) - 1;
            float coorY_1 = sepY * (std::min(obstacle.y, lim.y) + 1) - 1;

            float coorX_2 = sepX * (std::max(obstacle.x, lim.x) + 1) - 1;
            float coorY_2 = sepY * (std::max(obstacle.y, lim.y) + 1) - 1;

            glColor3f(0.5f, 0.5f, 0.5f);
            glVertex2f(coorX_1 - sepX / 3.0f, coorY_1 - sepY / 3.0f);
            glVertex2f(coorX_2 + sepX / 3.0f, coorY_1 - sepY / 3.0f);
            glVertex2f(coorX_2 + sepX / 3.0f, coorY_2 + sepY / 3.0f);
            glVertex2f(coorX_1 - sepX / 3.0f, coorY_2 + sepY / 3.0f);
        }
        for (auto& obs : obstacles)
        {
            CMatrix::Node lim(obs.state, matrix);

            float coorX_1 = sepX * (std::min(obs.x, lim.x) + 1) - 1;
            float coorY_1 = sepY * (std::min(obs.y, lim.y) + 1) - 1;

            float coorX_2 = sepX * (std::max(obs.x, lim.x) + 1) - 1;
            float coorY_2 = sepY * (std::max(obs.y, lim.y) + 1) - 1;

            glColor3f(1, 1, 1);

            glColor3f(1, 1, 0.0f);
            glVertex2f(coorX_1 - sepX / 3.0f, coorY_1 - sepY / 3.0f);

            glColor3f(1, 1, 0.2f);
            glVertex2f(coorX_2 + sepX / 3.0f, coorY_1 - sepY / 3.0f);

            glColor3f(1, 1, 0.4f);
            glVertex2f(coorX_2 + sepX / 3.0f, coorY_2 + sepY / 3.0f);

            glColor3f(1, 1, 0.8f);
            glVertex2f(coorX_1 - sepX / 3.0f, coorY_2 + sepY / 3.0f);
        }
        glColor3f(1, 1, 1);
        for (int x = 0; x < matrix.width; x++)
        {
            for (int y = 0; y < matrix.height; y++)
            {
                if (matrix.getNode(x, y))
                {
                    float coorX = sepX * (x + 1) - 1;
                    float coorY = sepY * (y + 1) - 1;
                    if (matrix.search)
                    {
                        if (std::find(matrix.search->nDisplay().begin(), matrix.search->nDisplay().end(), CMatrix::Node(x, y)) != matrix.search->nDisplay().end())
                            glColor3f(matrix.search->state == 2, 0, matrix.search->state != 2);
                        if (std::find(matrix.search->nFound().begin(), matrix.search->nFound().end(), CMatrix::Node(x, y)) != matrix.search->nFound().end())
                            glColor3f(matrix.search->state == 2 ? 1 : 0, matrix.search->state == 1 ? 1 : 0, !matrix.search->state);
                        if (CMatrix::Node(x, y) == matrix.search->target)
                            glColor3f(0, 1, 0);
                    }
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
