#pragma once

#include <vector>
#include <cassert>
struct Edge 
{
	int vert1;
	int vert2;
};

struct Graph 
{
	std::vector<std::vector<int>> neighbours;
	void addEdge(int vert1, int vert2)
	{
		assert(vert1 < neighbours.size());
		assert(vert2 < neighbours.size());
		neighbours[vert1].push_back(vert2);
		neighbours[vert2].push_back(vert1);
	}
	void addVertex()
	{
		neighbours.push_back(std::vector<int>());
	}
};