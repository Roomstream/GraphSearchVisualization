#include "editor.hpp"
#include <string>
#include "raymath.h"
#include "raygui.h"

void Editor::processCurrentAction() 
{
    Vector2 mousePosition = GetMousePosition();
    int hoveredVertex = getHoveredVertex();
    Edge hoveredEdge = getHoveredEdge();

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
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && hoveredVertex >= 0)
        {
            m_contextMenuData.rightClickPos = mousePosition;
            m_contextMenuData.oldHoveredVertex = hoveredVertex;
            m_contextMenuData.type = ContextMenuType::Vertex;
            m_currentAction = Action::ContextMenu;
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && hoveredEdge.vert1 >= 0) 
        {
            m_contextMenuData.rightClickPos = mousePosition;
            m_contextMenuData.oldHoveredEdge = hoveredEdge;
            m_contextMenuData.type = ContextMenuType::Edge;
            m_currentAction = Action::ContextMenu;
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
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            m_currentAction = Action::None;
            if (hoveredVertex != -1)
            {
                m_graph.addEdge(hoveredVertex, m_createEdgeData.vertex);
            }
        }
    }
    else if (m_currentAction == Action::ContextMenu)
    {
        Rectangle rectangle{
            m_contextMenuData.rightClickPos.x,
            m_contextMenuData.rightClickPos.y, 120, 30 };
        if (m_contextMenuData.type == ContextMenuType::Edge)
        {
            if (GuiButton(rectangle, "Delete edge"))
            {
                m_graph.eraseEdge(m_contextMenuData.oldHoveredEdge.vert1, m_contextMenuData.oldHoveredEdge.vert2);
                m_currentAction = Action::None;
            }
        }
        if (m_contextMenuData.type == ContextMenuType::Vertex)
        {
            if (GuiButton(rectangle, "Set start"))
            {
                start = m_contextMenuData.oldHoveredVertex;
                m_currentAction = Action::None;
            }
            rectangle.y = rectangle.y + 30;
            if (GuiButton(rectangle, "Delete vertex"))
            {
                m_vertexCoords.erase(m_vertexCoords.begin() + m_contextMenuData.oldHoveredVertex);
                m_graph.eraseVert(m_contextMenuData.oldHoveredVertex);
                m_currentAction = Action::None;
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)) 
        {
            m_currentAction = Action::None;
        }
    }
}

void Editor::tick()
{
    Vector2 mousePosition = GetMousePosition();
    int hoveredVertex = getHoveredVertex();
    Edge hoveredEdge = getHoveredEdge();

    for (int i = 0; i < m_graph.neighbours.size(); i++)
    {
        for (int j = 0; j < m_graph.neighbours[i].size(); j++)
        {
            DrawLineEx(m_vertexCoords[i], m_vertexCoords[m_graph.neighbours[i][j]], EDGE_WIDTH, BEIGE);
        }
    }
    if (hoveredVertex == -1 && hoveredEdge.vert1 > -1 && hoveredEdge.vert2 > -1)
    {
        DrawLineEx(m_vertexCoords[hoveredEdge.vert1], m_vertexCoords[hoveredEdge.vert2], EDGE_WIDTH, BROWN);
    }
    if (m_currentAction == Action::CreateEdge) 
    {
        DrawLineEx(m_vertexCoords[m_createEdgeData.vertex], mousePosition, EDGE_WIDTH, BEIGE);
    }
    for (int i = 0; i < m_vertexCoords.size(); i++)
    {
        Vector2 ballCenter = m_vertexCoords[i];
        float radius = VERTEX_RADIUS;
        Color color = BEIGE;
        
        DrawCircleV(ballCenter, VERTEX_RADIUS, color);
        std::string label = std::to_string(i);
        Vector2 size = MeasureTextEx(GetFontDefault(), label.c_str(), FONT_SIZE, 1);
        DrawText(label.c_str(), ballCenter.x - size.x / 2, ballCenter.y - size.y / 2, FONT_SIZE, BLACK);
    }
    if (hoveredVertex != -1)
    {
        Vector2 ballCenter = m_vertexCoords[hoveredVertex];
        DrawCircleV(ballCenter, VERTEX_RADIUS, BROWN);
        std::string label = std::to_string(hoveredVertex);
        Vector2 size = MeasureTextEx(GetFontDefault(), label.c_str(), FONT_SIZE, 1);
        DrawText(label.c_str(), ballCenter.x - size.x / 2, ballCenter.y - size.y / 2, FONT_SIZE, BLACK);
    }

    printVertices();

    processCurrentAction();
}

int Editor::getHoveredVertex()
{
    Vector2 mousePosition = GetMousePosition();
    for (int i = 0; i < m_vertexCoords.size(); i++)
    {
        Vector2 ballCenter = m_vertexCoords[i];
        if (Vector2Distance(ballCenter, mousePosition) <= VERTEX_RADIUS)
        {
            return i;
        }
    }

    return -1;
}
Edge Editor::getHoveredEdge() 
{
    for (int i = 0; i < m_graph.neighbours.size(); i++)
    {
        for (int j = 0; j < m_graph.neighbours[i].size(); j++)
        {
            if (CheckCollisionPointLine(GetMousePosition(),
                m_vertexCoords[i], m_vertexCoords[m_graph.neighbours[i][j]], EDGE_WIDTH / 2.f))
            {
                Edge edge;
                edge.vert1 = i;
                edge.vert2 = m_graph.neighbours[i][j];
                return edge;
            }

        }
    }
    Edge edge;
    edge.vert1 = -1;
    edge.vert2 = -1;
    return edge;
}

void Editor::printVertices()
{
    const Vector2 text_size = MeasureTextEx(GetFontDefault(), " ", FONT_SIZE, 1);
    for (int i = 0; i < m_vertexCoords.size(); i++)
    {
        Vector2 ballCenter = m_vertexCoords[i];
        std::string text = std::to_string(ballCenter.x) + " " + std::to_string(ballCenter.y);
        DrawText(text.c_str(), 3, 3 + text_size.y * (i + 1), FONT_SIZE, BLACK);
    }
}
