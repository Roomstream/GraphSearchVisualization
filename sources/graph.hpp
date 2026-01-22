#pragma once

#include <vector>
#include <cassert>
#include <functional>
struct Edge 
{
	int vert1;
	int vert2;
};

struct Graph 
{
	std::vector<std::vector<int>> neighbours;
	bool hasEdge(int vert1, int vert2)
	{
		for (int i = 0; i < neighbours[vert1].size(); i++)
		{
			if (neighbours[vert1][i] == vert2) 
			{
				return true;
			}
		}
		return false;
	}
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
	void eraseVert(int idx) 
	{
		neighbours.erase(neighbours.begin() + idx);
		for (int i = 0; i < neighbours.size(); i++)
		{
			std::erase(neighbours[i], idx);
			for (int& value : neighbours[i]) 
			{
				if (value > idx)
				{
					value = value - 1;
				}
			}
		}
	}
	void eraseEdge(int vert1, int vert2) 
	{
		std::erase(neighbours[vert1], vert2);
		std::erase(neighbours[vert2], vert1);
	}
	const std::vector<int>& getNeighbours(int vert) const
	{
		return neighbours[vert];
	}
	int vertCount() const 
	{
		return neighbours.size();
	}
};
void DFS2(int currVert, const Graph& graph, std::vector<bool>& isVisited,
	const std::function<void (int)>& onEnter, const std::function<void(int)>& onLeave);
void DFS(int currVert, const Graph& graph, const std::function<void(int)>& onEnter,
	const std::function<void(int)>& onLeave);