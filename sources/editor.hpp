#pragma once

#include <vector>
#include "raylib.h"
#include "graph.hpp"
#include "search_visualizer.hpp"

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
	void processCurrentAction();

	enum class Action
	{
		None,
		MoveVertex,
		CreateEdge,
		ContextMenu,
		StepVisualization,
	};

	enum class ContextMenuType
	{
		Vertex,
		Edge,
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
	
	int start;
	//bool ifButtonPressed = false;
	std::vector<Vector2> m_vertexCoords;
	Graph m_graph;
	struct ContextMenuData
	{
		Vector2 rightClickPos;
		int oldHoveredVertex;
		Edge oldHoveredEdge;
		ContextMenuType type;
	};
	ContextMenuData m_contextMenuData;
	SearchVisualizer m_searchVisualizer;
	int m_step = 0;
};