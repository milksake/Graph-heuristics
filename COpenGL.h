#pragma once
#include <GLFW/glfw3.h>
#include <algorithm>
#include "CMatrix.h"

class COpenGL
{
	GLFWwindow* window;
	int frame_time;
	int frame_count;
	int current_frame;

	void draw(const CMatrix& matrix);

public:
	COpenGL(int frameT = 0x0000000E);
	bool init(int windowW, int windowH);

	void run(CMatrix& matrix);
};
