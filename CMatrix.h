#pragma once

class CMatrix
{
	bool* matrix;

public:
	int width;
	int height;
	CMatrix(int width, int height);
	~CMatrix();

	void init();
	int getNode(int x, int y) const;
	void checkSiblings(int x, int y, bool* check) const;
	bool checkNode(int x, int y) const;
};
