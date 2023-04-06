#pragma once
#include <vector>
#include <iostream>

class CSearch;

class CMatrix
{
	bool* matrix;
	
public:
	CSearch* search = nullptr;

	struct Node {
		int x, y, state;
		Node(): x(0), y(0), state(0) {}
		Node(int index, const CMatrix& matrix) : state(0) { matrix.getCoord(index, x, y); }
		Node(int x, int y): x(x), y(y), state(0) {}
		Node(int x, int y, int state) : x(x), y(y), state(state) {}
		Node(const Node& n, int state) : x(n.x), y(n.y), state(state) {}
		bool operator== (const Node& rhs) const { return x == rhs.x && y == rhs.y; }
	};

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
	bool checkNode(Node n) const;
	bool checkRange(Node lim1, Node lim2, Node value) const;
	void resize(int percent);
	void eraseRange(Node a, Node b);

	std::vector<Node> selected_nodes;

	void update();

	template <typename T>
	void beginSearch(const Node& a, const Node& b)
	{
		selected_nodes.clear();
		if (search)
			delete search;
		search = new T(a, b, this);
	}
};
