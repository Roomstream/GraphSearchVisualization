#pragma once

#include <vector>
#include "raylib.h"
#include "graph.hpp"

static const float EDGE_WIDTH = 10.0f;
static const float VERTEX_RADIUS = 50.0f;
static const float FONT_SIZE = 20.0f;
class Editor
{
public:
	void tick();
private:

	int getHoveredVertex();
	Edge getHoveredEdge();
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