#pragma once

#include <vector>
#include <list>
#include <stack>

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

int EdgeClass(int x, int y)
{
	if (sup::parent[y] == x)
		return View::tree;
	else if (sup::discovered[y] && !sup::processed[y])
		return View::back;
	else if (sup::processed[y] && sup::entryTime[y] > sup::entryTime[x])
		return View::forward;
	else if (sup::processed[y] && sup::entryTime[y] < sup::entryTime[x])
		return View::cross;

	return -1;
}

void ProcessEdge(int x, int y)
{
	int edgeClass = EdgeClass(x, y);
	if (edgeClass == View::back)
		if (sup::entryTime[y] < sup::entryTime[sup::low[x]])
			sup::low[x] = y;

	if (edgeClass == View::cross)
		if (sup::scc[y] == -1 && sup::entryTime[y] < sup::entryTime[sup::low[x]])
			sup::low[x] = y;
}

void ProcessTopEarly(int v)
{
	sup::stack.push(v);
}

void ProcessTopLate(int v)
{
	if (sup::low[v] == v)
	{
		int t;
		sup::componentCount++;
		sup::scc[v] = sup::componentCount;
		while ((t = sup::stack.top()) != v)
		{
			sup::stack.pop();
			sup::scc[t] = sup::componentCount;
		}
		if (!sup::stack.empty())
			sup::stack.top();
	}
	if (sup::parent[v] > 0)
		if (sup::entryTime[sup::low[v]] <
			sup::entryTime[sup::low[sup::parent[v]]]) {

			sup::low[sup::parent[v]] = sup::low[v];
		}

}

void StrongComponents(const std::vector<std::list<int>>& graph)
{
	for (int i = 0; i < graph.size(); i++)
	{
		sup::low[i] = i;
		sup::scc[i] = -1;
	}

	sup::componentCount = 0;

}

void _DFS(const std::vector<std::list<int>>& graph, int top)
{
	sup::discovered[top] = true;

	//

	sup::entryTime[top] = sup::time;
	sup::time++;
	for (int t : graph[top])
	{
		//
		if (!sup::discovered[t])
		{
			sup::parent[t] = top;
			_DFS(graph, t);
		}
	}
	sup::processed[top] = true;
	sup::time++;
}



std::list<int> DFS(const std::vector<std::list<int>>& graph, int countEdge)
{
	sup::discovered.resize(graph.size(), false);
	sup::processed.resize(graph.size(), false);
	sup::parent.resize(graph.size(), -1);
	sup::entryTime.resize(graph.size(), 0);
	sup::low.resize(countEdge);
	sup::scc.resize(countEdge);

	for (int i = 0; i < graph.size(); i++)
	{
		_DFS(graph, 0);
		if (sup::ans.empty())
			return sup::ans;
		sup::processed.assign(sup::processed.size(), false);
		sup::discovered.assign(sup::discovered.size(), false);
	}
	return sup::ans;
}
