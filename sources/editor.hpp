#pragma once

#include <vector>
#include "raylib.h"
#include "graph.hpp"


class Editor
{
public:
	void tick();
private:

	int getHoveredVertex();
	void printVertices();

	enum class Action
	{
		None,
		MoveVertex,
		CreateEdge,
	};

	Action m_currentAction = Action::None;

	struct VertexMoveData
	{
		int vertex;
		Vector2 startPosCursor;
		Vector2 startPosVertex;
	};
	VertexMoveData m_vertexMove;

	struct CreateEdgeData
	{
		int vertex;
	};
	CreateEdgeData m_createEdgeData;

	std::vector<Vector2> m_vertexCoords;
	Graph m_graph;

};