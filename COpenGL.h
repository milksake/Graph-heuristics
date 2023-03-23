#pragma once
#include <GLFW/glfw3.h>
#include "CMatrix.h"

class COpenGL
{
	GLFWwindow* window;

	void draw(const CMatrix& matrix);

public:
	COpenGL();
	bool init(int windowW, int windowH);

	void run(const CMatrix& matrix);
};
