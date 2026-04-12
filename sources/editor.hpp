#pragma once

#include <sstream>
#include <vector>
#include "raylib.h"
#include "graph.hpp"
#include "search_visualizer.hpp"

static const float EDGE_WIDTH = 10.0f;
static const float VERTEX_RADIUS = 50.0f;

static const float FONT_SIZE = 40.0f;


class Editor
{
public:
	void tick();
private:
	int getHoveredVertex();
	Edge getHoveredEdge();
	void printVertices();
	void processCurrentAction();
	void save();
	void load();
	void load(std::stringstream& data);

	enum class Action
	{
		None,
		MoveVertex,
		CreateEdge,
		ContextMenu,
		StepVisualization,
		MoveCamera,
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
	
	int start = 0;
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

	struct MoveCameraData
	{
		Vector2 startPosCursor;
	};
	MoveCameraData m_moveCameraData;

	SearchVisualizer m_searchVisualizer;
	int m_step = 0;
};