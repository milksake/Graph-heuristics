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
  static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods); //callback que se llamará cada vez que se presione un botón del mouse

public:

	COpenGL(int frameT = 0xE);
	bool init(int windowW, int windowH);

	void run(CMatrix& matrix);
};
