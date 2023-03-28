#pragma once
#include <vector>
#include <deque>
#include <array>

class CMatrix
{
	bool* matrix;
	
public:
	struct Node {
		int x, y, state;
		Node(): x(0), y(0), state(0) {}
		Node(int index, const CMatrix& matrix) : state(0) { matrix.getCoord(index, x, y); }
		Node(int x, int y): x(x), y(y), state(0) {}
		Node(int x, int y, int state) : x(x), y(y), state(state) {}
		Node(Node n, int state) : x(n.x), y(n.y), state(state) {}
		bool operator== (Node rhs) const { return x == rhs.x && y == rhs.y; }
	};

	int state; // 0 - nothing, 1 - DFS, 2 - BFS, 3 - success, 4 - failure
	std::vector<Node> DFS_path;
	std::vector<Node> DFS_evaluated;
	std::deque<Node> BFS_queue;
	std::vector<Node> BFS_evaluated;
	Node target;
	int width;
	int height;

	CMatrix(int width, int height);
	~CMatrix();

	void init();
	bool getNode(int x, int y) const;
	void setNode(int x, int y, bool set);
	void getCoord(int index, int& x, int& y) const;
	int checkSiblings(int x, int y) const;
	int checkSiblings(Node n) const;
	int checkSomeSiblings(int x, int y) const;
	bool checkNode(int x, int y) const;

	void update();
	void beginDFS(const Node& a, const Node& b);
	void endDFS(bool s);
	void beginBFS(const Node& a, const Node& b);
	void endBFS(bool s);
	
	void DFS();
	void BFS();
};
