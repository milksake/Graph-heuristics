#include "CInput.h"
#include "CMatrix.h"
#include "COpenGL.h"

// Called every frame (when user input is permitted)
void CInput::update(COpenGL* context, CMatrix* matrix)
{
	// check input
	// check key presses and call matrix->resize
	// check mouse position (context->window) 
	// add nodes to matrix->selected_nodes if a node was clicked
	// call matrix->beginDFS(node, node) or beginBFS(node, node) if two nodes are in matrix->selected_nodes
	
	//ERASE
	matrix->resize(50);
	matrix->beginBFS(CMatrix::Node(rand() % matrix->width, rand() % matrix->height), CMatrix::Node(rand() % matrix->width, rand() % matrix->height));
	//matrix->selected_nodes.push_back(CMatrix::Node(12, 10));
	//END ERASE
}
