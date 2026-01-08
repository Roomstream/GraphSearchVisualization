#include "editor.hpp"
#include <string>
#include "raymath.h"


void Editor::tick()
{
    Vector2 mousePosition = GetMousePosition();
    int hoveredVertex = getHoveredVertex();

    if (m_currentAction == Action::None) 
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            m_vertexCoords.push_back(mousePosition);
            m_graph.addVertex();
        }
        else if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
            && hoveredVertex >= 0)
        {
            m_currentAction = Action::CreateEdge;
            m_createEdgeData.vertex = hoveredVertex;
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hoveredVertex >= 0)
        {
            m_vertexMove.vertex = hoveredVertex;
            m_vertexMove.startPosCursor = mousePosition;
            m_vertexMove.startPosVertex = m_vertexCoords[hoveredVertex];
            m_currentAction = Action::MoveVertex;
        }
    }
    else if (m_currentAction == Action::MoveVertex) 
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            m_currentAction = Action::None;
        }
        Vector2 offset = mousePosition - m_vertexMove.startPosCursor;
        m_vertexCoords[m_vertexMove.vertex] = offset + m_vertexMove.startPosVertex;
        hoveredVertex = m_vertexMove.vertex;
    }
    else if (m_currentAction == Action::CreateEdge)
    {
        DrawLineEx(m_vertexCoords[m_createEdgeData.vertex], mousePosition, 10, BEIGE);
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            m_currentAction = Action::None;
            if (hoveredVertex != -1)
            {
                m_graph.addEdge(hoveredVertex, m_createEdgeData.vertex);
            }
        }
    }

    for (int i = 0; i < m_graph.neighbours.size(); i++)
    {
        for (int j = 0; j < m_graph.neighbours[i].size(); j++)
        {
            DrawLineEx(m_vertexCoords[i], m_vertexCoords[m_graph.neighbours[i][j]], 10, BEIGE);
        }
    }

    for (int i = 0; i < m_vertexCoords.size(); i++)
    {
        Vector2 ballCenter = m_vertexCoords[i];
        float radius = 50;
        Color color = BEIGE;
        
        DrawCircleV(ballCenter, 50, color);
        std::string label = std::to_string(i);
        Vector2 size = MeasureTextEx(GetFontDefault(), label.c_str(), 20, 1);
        DrawText(label.c_str(), ballCenter.x - size.x / 2, ballCenter.y - size.y / 2, 20, BLACK);
    }
    if (hoveredVertex != -1)
    {
        Vector2 ballCenter = m_vertexCoords[hoveredVertex];
        DrawCircleV(ballCenter, 50, BROWN);
        std::string label = std::to_string(hoveredVertex);
        Vector2 size = MeasureTextEx(GetFontDefault(), label.c_str(), 20, 1);
        DrawText(label.c_str(), ballCenter.x - size.x / 2, ballCenter.y - size.y / 2, 20, BLACK);
    }

    printVertices();
}

int Editor::getHoveredVertex()
{
    Vector2 mousePosition = GetMousePosition();
    float radius = 50;
    for (int i = 0; i < m_vertexCoords.size(); i++)
    {
        Vector2 ballCenter = m_vertexCoords[i];
        if (Vector2Distance(ballCenter, mousePosition) <= radius)
        {
            return i;
        }
    }

    return -1;
}

void Editor::printVertices()
{
    const Vector2 text_size = MeasureTextEx(GetFontDefault(), " ", 20, 1);
    for (int i = 0; i < m_vertexCoords.size(); i++)
    {
        Vector2 ballCenter = m_vertexCoords[i];
        std::string text = std::to_string(ballCenter.x) + " " + std::to_string(ballCenter.y);
        DrawText(text.c_str(), 3, 3 + text_size.y * (i + 1), 20, BLACK);
    }
}
