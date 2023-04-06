#pragma once
#include <GLFW/glfw3.h>
#include <algorithm>
#include "CMatrix.h"
#include "CInput.h"

class COpenGL
{
	GLFWwindow* window;
	CInput input;
	int frame_time;
	int frame_count;
	int current_frame;

	int wWidth, wHeight;

	void draw(const CMatrix& matrix);

public:
	std::vector<CMatrix::Node> obstacles;
	CMatrix::Node obstacle;
	void convertToGLCoor(double& x, double& y);
	CMatrix::Node getNodePosition(float x, float y);

	COpenGL(int frameT = 0xE);
	bool init(int windowW, int windowH);

	void run(CMatrix& matrix);
};
