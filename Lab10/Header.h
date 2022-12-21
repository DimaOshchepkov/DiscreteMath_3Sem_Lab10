#pragma once

#include <vector>
#include <list>

enum View
{
	tree,
	back,
	forward,
	cross,
};

int EdgeClass(int x, int y, const std::vector<int>& parent,
	const std::vector<bool>& visited, const std::vector<bool>& open,
	const std::vector<int>& entryTime)
{
	if (parent[y] == x) return View::tree;
	else if (visited[y] && !open[y]) return View::back;
	else if (open[y] && entryTime[y] > entryTime[x]) return View::forward;
	else if (open[y] && entryTime[y] < entryTime[x]) return View::cross;

	return -1;
}

void ProcessedEdge(int x, int y, std::vector<int>& low, std::vector<int>& scc,
	const std::vector<int>& entryTime, const std::vector<int>& parent,
	const std::vector<bool>& visited, const std::vector <bool> & open)
{
	int edgeClass = EdgeClass(x, y, parent, visited, open, entryTime);
	if (edgeClass == View::back)
		if (entryTime[y] < entryTime[low[x]])
			low[x] = y;

	if (edgeClass == View::cross)
		if (scc[y] == -1 && entryTime[y] < entryTime[low[x]])
			low[x] = y;
}

void _DFS(const std::vector<std::list<int>>& graph, std::vector<bool>& open, std::vector<bool>& visited, std::vector<int>& parent,
	int top, std::vector<int> entryTime, int time,
	std::vector<int>& low, std::vector<int>& scc)
{
	open[top] = true;
	entryTime[top] = time;
	time++;
	for (int t : graph[top])
	{
		if (!open[t])
		{
			parent[t] = top;

			_DFS(graph, open, visited, parent, t, entryTime, time);
		}
		else
		{

		}
	}
	if (low[top] == top)

	visited[top] = true;
}



std::list<int> DFS(const std::vector<std::list<int>>& graph, int countEdge)
{
	std::vector<bool> visited(graph.size(), false);
	std::vector<bool> open(graph.size(), false);
	std::vector<int> parent(graph.size(), -1);
	std::vector<int> time(graph.size(), 0);
	std::vector<int> low(countEdge);
	std::vector<int> scc(countEdge);
	std::list<int> ans;

	for (int i = 0; i < graph.size(); i++)
	{
		_DFS(graph, open, visited, parent, i, time, 0, low, scc);
		if (ans.size() != 0)
			return ans;
		open.assign(open.size(), false);
		visited.assign(visited.size(), false);
	}
	return ans;
}
