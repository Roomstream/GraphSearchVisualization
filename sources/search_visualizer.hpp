#pragma once

#include <vector>
#include "graph.hpp"


class SearchVisualizer 
{
public:

	enum class VertexState
	{
		nonProcessed,
		partiallyProcessed,
		processed,
	};

	struct SearchStep
	{
		enum class Type
		{
			EnterVertex,
			LeaveVertex,
		};
		Type type;
		int vertex;
		std::vector<VertexState> m_vertexStates;
	};
	
	std::vector<SearchStep> m_steps;
	void DFS2(int currVert, const Graph& graph, std::vector<VertexState>& vertexStates)
	{
		vertexStates[currVert] = VertexState::partiallyProcessed;

		SearchStep step;
		step.type = SearchStep::Type::EnterVertex;
		step.m_vertexStates = vertexStates;
		step.vertex = currVert;
		m_steps.push_back(step);

		const std::vector<int>& neighbours = graph.getNeighbours(currVert);
		for (int neighbour : neighbours)
		{
			if (vertexStates[neighbour] == VertexState::nonProcessed)
			{
				DFS2(neighbour, graph, vertexStates);
				step.m_vertexStates = vertexStates;
				step.type = SearchStep::Type::LeaveVertex;
				m_steps.push_back(step);
			}
		}

		vertexStates[currVert] = VertexState::processed;
	}

	void DFS(int currVert, const Graph& graph)
	{
		clear();
		std::vector<VertexState> vertexStates(graph.vertCount(), VertexState::nonProcessed);
		DFS2(currVert, graph, vertexStates);
	}

	void clear() 
	{
		m_steps.clear();
	}

	int getStepsNum() 
	{
		return m_steps.size();
	}

	VertexState getStateForVertex(int step, int vert)
	{
		return m_steps[step].m_vertexStates[vert];
	}
	int getCurrVertex(int step)
	{
		return m_steps[step].vertex;
	}
};