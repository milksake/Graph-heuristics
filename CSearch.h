#pragma once
#include "CMatrix.h"
#include <deque>
#include <map>

class CSearch
{
public:
	CSearch(CMatrix* matrix);

	CMatrix* matrix;
	CMatrix::Node target;

	int state; // 0 - searching, 1 - found, 2 - not found

	virtual std::vector<CMatrix::Node>& nFound() = 0;
	virtual std::vector<CMatrix::Node>& nDisplay() = 0;

	virtual void update() = 0;
	virtual void begin(const CMatrix::Node& a, const CMatrix::Node& b) = 0;
	virtual void end(bool s) = 0;
};

class CSearchDFS : public CSearch
{
	std::vector<CMatrix::Node> DFS_path;
	std::vector<CMatrix::Node> DFS_evaluated;
	std::vector<CMatrix::Node> empty;

public:
	CSearchDFS(const CMatrix::Node& a, const CMatrix::Node b, CMatrix* matrix);

	// Inherited via CSearch
	virtual std::vector<CMatrix::Node>& nFound() override;
	virtual std::vector<CMatrix::Node>& nDisplay() override;

	// Inherited via CSearch
	virtual void update() override;
	virtual void begin(const CMatrix::Node& a, const CMatrix::Node& b) override;
	virtual void end(bool s) override;
};

class CSearchBFS : public CSearch
{
	std::deque<CMatrix::Node> BFS_queue;
	std::vector<CMatrix::Node> BFS_evaluated;
	std::vector<CMatrix::Node> BFS_path;

public:
	CSearchBFS(const CMatrix::Node& a, const CMatrix::Node b, CMatrix* matrix);

	// Inherited via CSearch
	virtual std::vector<CMatrix::Node>& nFound() override;
	virtual std::vector<CMatrix::Node>& nDisplay() override;

	// Inherited via CSearch
	virtual void update() override;
	virtual void begin(const CMatrix::Node& a, const CMatrix::Node& b) override;
	virtual void end(bool s) override;
};

class CSearchBestFirstS : public CSearch
{
protected:
	std::multimap<int, CMatrix::Node> open;
	std::vector<CMatrix::Node> closed;
	std::vector<CMatrix::Node> path;
	
	virtual float heuristic(CMatrix::Node x);
	bool searchMap(CMatrix::Node n);

public:
	CSearchBestFirstS(const CMatrix::Node& a, const CMatrix::Node b, CMatrix* matrix);

	// Inherited via CSearch
	virtual std::vector<CMatrix::Node>& nFound() override;
	virtual std::vector<CMatrix::Node>& nDisplay() override;

	virtual void update() override;
	virtual void begin(const CMatrix::Node& a, const CMatrix::Node& b) override;
	virtual void end(bool s) override;
};
