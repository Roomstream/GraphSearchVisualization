#include "graph.hpp"

void DFS2(int currVert, const Graph& graph, std::vector<bool>& isVisited, 
	const std::function<void(int)>& onEnter, const std::function<void(int)>& onLeave)
{
	onEnter(currVert);
	isVisited[currVert] = true;
	const std::vector<int>& neighbours = graph.getNeighbours(currVert);
	for (int neighbour : neighbours)
	{
		if (isVisited[neighbour] == false)
		{
			DFS2(neighbour, graph, isVisited, onEnter, onLeave);
		}
	}
	onLeave(currVert);
}

void DFS(int currVert, const Graph& graph, const std::function<void(int)>& onEnter, 
	const std::function<void(int)>& onLeave)
{
	std::vector<bool> isVisited(graph.vertCount(), false);
	DFS2(currVert, graph, isVisited, onEnter, onLeave);
}
