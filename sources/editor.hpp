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

	struct VertexMoveAction
	{
		int vertex = -1;
		Vector2 startPosCursor;
		Vector2 startPosVertex;
	};
	VertexMoveAction m_vertexMove;
};