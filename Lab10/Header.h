#pragma once

#include <vector>
#include <list>
#include <stack>
#include <algorithm>


int EdgeClass(int x, int y);

std::vector<int> DFS(const std::vector<std::list<int>>& graph);

void InputGraph(const std::string& path = "graph.txt");

std::vector<std::list<int>> ReadGraph(const std::string& path = "graph.txt");
