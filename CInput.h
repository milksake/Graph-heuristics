#pragma once

class COpenGL;
class CMatrix;

class CInput
{
public:
	COpenGL* context = nullptr;
	CMatrix* matrix = nullptr;

	//CInput();

	void update(COpenGL* context, CMatrix* matrix);

};
