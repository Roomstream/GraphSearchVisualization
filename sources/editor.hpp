#pragma once

#include <vector>
#include "raylib.h"


class Editor
{
public:
	void tick();
private:

	int getHoveredVertex();
	void printVertices();
	std::vector<Vector2> m_ballCenters;

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

};