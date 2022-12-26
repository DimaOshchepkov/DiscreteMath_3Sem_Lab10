#include <fstream>
#include <string>
#include <iostream>

#include "Header.h"

enum View
{
	tree,
	back,
	forward,
	cross,
};

namespace sup
{
	std::vector<bool> discovered;
	std::vector<bool> processed;
	std::vector<int> parent;
	std::vector<int> entryTime;
	std::vector<int> low;
	std::vector<int> scc;
	std::list<int> ans;
	std::stack<int> stack;
	int time = 0;
	int componentCount = 0;
};

class Sup
{
public:
	std::vector<bool> discovered;
	std::vector<bool> processed;
	std::vector<int> parent;
	std::vector<int> entryTime;
	std::vector<int> low;
	std::vector<int> scc;
	std::stack<int> stack;
	int time = 0;
	int componentCount = 0;

	Sup(int graphSize)
		: discovered(graphSize, false)
		, processed(graphSize, false)
		, parent(graphSize, -1)
		, entryTime(graphSize, -1)
		, low(graphSize, -1)
		, scc(graphSize, -1)
		, stack()
		, time(0)
		, componentCount(0) {}

	~Sup()
	{
		discovered.clear();
		processed.clear();
		parent.clear();
		entryTime.clear();
		low.clear();
		scc.clear();

		discovered.shrink_to_fit();
		processed.shrink_to_fit();
		parent.shrink_to_fit();
		entryTime.shrink_to_fit();
		low.shrink_to_fit();
		scc.shrink_to_fit();
	}

};

int EdgeClass(int x, int y, Sup& sup)
{
	if (sup.parent[y] == x)
		return View::tree;
	else if (sup.discovered[y] && !sup.processed[y])
		return View::back;
	else if (sup.processed[y] && sup.entryTime[y] > sup.entryTime[x])
		return View::forward;
	else if (sup.processed[y] && sup.entryTime[y] < sup.entryTime[x])
		return View::cross;

	return -1;
}

void ProcessEdge(int x, int y, Sup& sup)
{
	int edgeClass = EdgeClass(x, y, sup);
	if (edgeClass == View::back)
		if (sup.entryTime[y] < sup.entryTime[sup.low[x]])
			sup.low[x] = y;

	if (edgeClass == View::cross)
		if (sup.scc[y] == -1 && sup.entryTime[y] < sup.entryTime[sup.low[x]])
			sup.low[x] = y;
}

void ProcessTopEarly(int v, Sup& sup)
{
	sup.stack.push(v);
}

void ProcessTopLate(int v, Sup& sup)
{
	if (sup.low[v] == v)
	{
		int t;
		sup.componentCount++;
		sup.scc[v] = sup.componentCount;
		while ((t = sup.stack.top()) != v)
		{
			sup.stack.pop();
			sup.scc[t] = sup.componentCount;
		}
		if (!sup.stack.empty())
			sup.stack.pop();
	}
	if (sup.parent[v] >= 0)
		if (sup.entryTime[sup.low[v]] < sup.entryTime[sup.low[sup.parent[v]]]) {
			sup.low[sup.parent[v]] = sup.low[v];
		}

}


void _DFS(const std::vector<std::list<int>>& graph, int top, Sup& sup)
{
	sup.discovered[top] = true;

	ProcessTopEarly(top, sup);

	sup.entryTime[top] = sup.time;
	sup.time++;
	for (int t : graph[top])
	{
		if (!sup.discovered[t])
		{
			sup.parent[t] = top;
			_DFS(graph, t, sup);
		}
		ProcessEdge(top, t, sup);
	}
	ProcessTopLate(top, sup);
	sup.processed[top] = true;
	sup.time++;
}



std::vector<int> DFS(const std::vector<std::list<int>>& graph)
{
	Sup sup(graph.size());

	for (int i = 0; i < graph.size(); i++)
	{
		sup.low[i] = i;
		sup.scc[i] = -1;
	}

	sup.componentCount = -1;

	for (int i = 0; i < graph.size(); i++)
		if (!sup.discovered[i])
			_DFS(graph, i, sup);

	return sup.scc;
}

void InputGraph(const std::string& path)
{
	std::cout << "Input count top\n";
	int countTop = 0;
	std::cin >> countTop;

	std::cout << "Input count edge\n";
	int countEdge = 0;
	std::cin >> countEdge;

	std::cout << "Input graph (top1 -> top2)\n";

	std::ofstream fout(path);
	fout << countTop << " " << countEdge << '\n';
	int t1, t2;
	for (int i = 0; i < countEdge; i++)
	{
		std::cin >> t1 >> t2;
		fout << t1 << " " << t2 << '\n';
	}

	fout.close();
}

std::vector<std::list<int>> ReadGraph(const std::string& path)
{
	std::ifstream fin(path);

	int countTop, countEdge;
	fin >> countTop >> countEdge;
	std::vector<std::list<int>> graph(countTop, std::list<int>());
	int t1, t2;
	while (fin >> t1 >> t2)
	{
		graph[t1].push_back(t2);
	}

	fin.close();

	return graph;
}