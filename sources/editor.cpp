#include "editor.hpp"
#include <string>
#include "raymath.h"
#include "raygui.h"
#include <iostream>
#include "search_visualizer.hpp"
#include <fstream>
#include <sstream>

std::vector<std::string> embeddedGraphs = {
R"""(
7
1122 472
996 538
916 653
756 695
711 514
860 462
535 485
0 1: 1 
1 3: 0 2 5 
2 3: 1 3 5 
3 2: 2 4 
4 3: 3 5 6 
5 3: 4 2 1 
6 1: 4 
)""",
R"""(
11
542 511
767 507
771 710
763 323
959 707
954 324
967 493
1160 317
1164 492
1170 713
1375 497
0 3: 3 1 2
1 2 : 0 6
2 2 : 0 4
3 3 : 0 5 6
4 3 : 2 9 8
5 2 : 3 7
6 3 : 1 3 8
7 2 : 5 10
8 3 : 6 4 10
9 2 : 4 10
10 3 : 7 8 9
)""",
R"""(
13
635 530
730 393
865 330
998 397
1084 552
1005 684
879 757
726 672
767 242
939 229
1075 153
1058 285
1185 267
0 2: 1 7 
1 3: 0 2 8 
2 4: 1 3 9 11 
3 2: 2 4 
4 2: 3 5 
5 2: 4 6 
6 2: 5 7 
7 2: 6 0 
8 1: 1 
9 2: 2 10 
10 1: 9 
11 2: 2 12 
12 1: 11 
)""",
};

Vector2 cameraPosition = Vector2(0, 0);
int moveCameraTicks = 0;

Vector2 GetWorldMousePos()
{
    return cameraPosition + GetMousePosition();
}

Vector2 GetScreenMousePos()
{
    return GetMousePosition();
}

Vector2 getScreenCoords(Vector2 pos)
{
    return pos - cameraPosition;
}

void Editor::processCurrentAction() 
{
    std::string text = std::to_string(cameraPosition.x) + " " + std::to_string(cameraPosition.y);
    DrawText(text.c_str(), 3, 3, 20, BLACK);
    Vector2 mousePosition = GetWorldMousePos();
    int hoveredVertex = getHoveredVertex();
    Edge hoveredEdge = getHoveredEdge();

    if (m_currentAction == Action::None)
    {
#if TARGET_PHONE
        int currentY = 30;
        std::string currentStepStr = "Current step: " + std::to_string(m_step);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), currentStepStr.c_str(), FONT_SIZE, 1);
        DrawText(currentStepStr.c_str(), (GetScreenWidth() - textSize.x) / 2.0, currentY, FONT_SIZE, BLACK);
        currentY += textSize.y;
        Rectangle rectangle;
        rectangle.x = 30;
        rectangle.y = currentY;
        rectangle.width = GetScreenWidth() - 60;
        rectangle.height = 30;
        static float stepSliderValue = 0.0f;
        GuiSlider(rectangle, "", nullptr, &stepSliderValue, 0, m_searchVisualizer.getStepsNum() - 1);
        m_step = round(stepSliderValue);

        int oldStart = start;
        std::string startingPointStr = "Starting point: " + std::to_string(start);
        textSize = MeasureTextEx(GetFontDefault(), startingPointStr.c_str(), FONT_SIZE, 1);
        currentY += 30;
        DrawText(startingPointStr.c_str(), (GetScreenWidth() - textSize.x) / 2.0, currentY, FONT_SIZE, BLACK);
        currentY += textSize.y;
        rectangle.y = currentY;
        static float startSliderValue = 0.0f;
        GuiSlider(rectangle, "", "", &startSliderValue, 0, m_vertexCoords.size() - 1);
        start = round(startSliderValue);

        static int graphIdx = -1;
        int oldGraphIdx = graphIdx;
        if (graphIdx == -1)
        {
            graphIdx = 0;
        }
        std::string graphStr = "Graph: " + std::to_string(graphIdx);
        textSize = MeasureTextEx(GetFontDefault(), graphStr.c_str(), FONT_SIZE, 1);
        currentY += textSize.y;
        DrawText(graphStr.c_str(), (GetScreenWidth() - textSize.x) / 2.0, currentY, FONT_SIZE, BLACK);
        currentY += textSize.y;
        rectangle.y = currentY;
        static float graphSliderValue = 0;
        static int result = 0;
        result = GuiSlider(rectangle, "", "", &graphSliderValue, 0, embeddedGraphs.size() - 1);
        graphIdx = round(graphSliderValue);

        if (m_vertexCoords.empty() || graphIdx != oldGraphIdx || oldStart != start)
        {
            m_step = stepSliderValue = 0.0f;
            startSliderValue = 0.0f;
            std::stringstream data(embeddedGraphs[graphIdx]);
            load(data);
            m_searchVisualizer.DFS(start, m_graph);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            m_currentAction = Action::MoveCamera;
            m_moveCameraData.startPosCursor = mousePosition;
            moveCameraTicks = 0;
        }
#else
        //Библиотека графов
        Rectangle rectangle{
           10,
           250, 120, 30 };

        if (GuiButton(rectangle, "save graph")) 
        {
            save();
            return;
        }
        rectangle.y += 30;
        if (GuiButton(rectangle, "load graph"))
        {
            load();
            return;
        }

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
            m_contextMenuData.rightClickPos = getScreenCoords(mousePosition);
            m_contextMenuData.oldHoveredVertex = hoveredVertex;
            m_contextMenuData.type = ContextMenuType::Vertex;
            m_currentAction = Action::ContextMenu;
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && hoveredEdge.vert1 >= 0) 
        {
            m_contextMenuData.rightClickPos = getScreenCoords(mousePosition);
            m_contextMenuData.oldHoveredEdge = hoveredEdge;
            m_contextMenuData.type = ContextMenuType::Edge;
            m_currentAction = Action::ContextMenu;
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            m_currentAction = Action::MoveCamera;
            m_moveCameraData.startPosCursor = mousePosition;
        }
#endif
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
            bool shouldAddEdge = hoveredVertex != -1 
                && !m_graph.hasEdge(hoveredVertex, m_createEdgeData.vertex)
                && hoveredVertex != m_createEdgeData.vertex;
            if (shouldAddEdge)
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
            if (GuiButton(rectangle, "Run DFS"))
            {
                m_step = 0;
                start = m_contextMenuData.oldHoveredVertex;
                m_currentAction = Action::StepVisualization;
                m_searchVisualizer.DFS(start, m_graph);
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
    else if (m_currentAction == Action::StepVisualization) 
    {
        Rectangle rectangle;
        rectangle.x = 100;
        rectangle.y = 100;
        rectangle.width = 300;
        rectangle.height = 30;
        GuiSpinner(rectangle, "Step", &m_step, 0, m_searchVisualizer.getStepsNum() - 1, false);

        if (IsKeyPressed(KEY_ESCAPE))
        {
            m_currentAction = Action::None;
            m_searchVisualizer.clear();
        }
    }
    else if (m_currentAction == Action::MoveCamera)
    {
        ++moveCameraTicks; // Без пропуска первых кадров на телефоне камера телепортируется
        if (moveCameraTicks > 5 && IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 drag = GetMouseDelta();
            cameraPosition -= drag;
        }

        if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            m_currentAction = Action::None;
        }
    }
}

void Editor::tick()
{
    Vector2 mousePosition = GetWorldMousePos();
    int hoveredVertex = getHoveredVertex();
    Edge hoveredEdge = getHoveredEdge();

    for (int i = 0; i < m_graph.neighbours.size(); i++)
    {
        for (int j = 0; j < m_graph.neighbours[i].size(); j++)
        {
            DrawLineEx(getScreenCoords(m_vertexCoords[i]), getScreenCoords(m_vertexCoords[m_graph.neighbours[i][j]]), EDGE_WIDTH, BEIGE);
        }
    }
    if (hoveredVertex == -1 && hoveredEdge.vert1 > -1 && hoveredEdge.vert2 > -1)
    {
        DrawLineEx(getScreenCoords(m_vertexCoords[hoveredEdge.vert1]), getScreenCoords(m_vertexCoords[hoveredEdge.vert2]), EDGE_WIDTH, BROWN);
    }
    if (m_currentAction == Action::CreateEdge)
    {
        DrawLineEx(getScreenCoords(m_vertexCoords[m_createEdgeData.vertex]), getScreenCoords(mousePosition), EDGE_WIDTH, BEIGE);
    }
    for (int i = 0; i < m_vertexCoords.size(); i++)
    {
        Vector2 ballCenter = getScreenCoords(m_vertexCoords[i]);
        float radius = VERTEX_RADIUS;
        Color color = BEIGE;

        if (m_searchVisualizer.getStepsNum() == 0)
        {
            DrawCircleV(ballCenter, VERTEX_RADIUS, color);
        }
        else
        {
            SearchVisualizer::VertexState state = m_searchVisualizer.getStateForVertex(m_step, i);
            if (state == SearchVisualizer::VertexState::partiallyProcessed)
            {
                color = YELLOW;
            }
            else if (state == SearchVisualizer::VertexState::processed)
            {
                color = GREEN;
            }
            if (m_searchVisualizer.getCurrVertex(m_step) == i)
            {
                color = BLUE;
            }
            DrawCircleV(ballCenter, VERTEX_RADIUS, color);
        }
        std::string label = std::to_string(i);
        Vector2 size = MeasureTextEx(GetFontDefault(), label.c_str(), FONT_SIZE, 1);
        DrawText(label.c_str(), ballCenter.x - size.x / 2, ballCenter.y - size.y / 2, FONT_SIZE, BLACK);
    }
    if (hoveredVertex != -1)
    {
        Vector2 ballCenter = getScreenCoords(m_vertexCoords[hoveredVertex]);
        DrawCircleV(ballCenter, VERTEX_RADIUS, BROWN);
        std::string label = std::to_string(hoveredVertex);
        Vector2 size = MeasureTextEx(GetFontDefault(), label.c_str(), FONT_SIZE, 1);
        DrawText(label.c_str(), ballCenter.x - size.x / 2, ballCenter.y - size.y / 2, FONT_SIZE, BLACK);
    }

    // printVertices();

    processCurrentAction();
}

int Editor::getHoveredVertex()
{
    Vector2 mousePosition = GetWorldMousePos();
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
            if (CheckCollisionPointLine(GetWorldMousePos(),
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
void Editor::save() 
{
    //Сохранение графа в файл
    std::ofstream out;
    out.open("graph.txt");
    if (out.is_open()) 
    {
        out << m_vertexCoords.size() << std::endl;
        for (int i = 0; i < m_vertexCoords.size(); i++) 
        {
            out << m_vertexCoords[i].x << ' ' << m_vertexCoords[i].y << std::endl;
        }
        for (int i = 0; i < m_graph.neighbours.size(); i++) 
        {
            out << i << " " << m_graph.neighbours[i].size() << ": ";
            for (int j = 0; j < m_graph.neighbours[i].size(); j++) 
            {
                out << m_graph.neighbours[i][j] << ' ';
                /*
                0 2: 1 2
                1 2: 0 2
                2 3: 3 1 0
                */
            }

            out << std::endl;
        }
    }
    out.close();
}
void Editor::load() 
{
    //Загрузка графа из файла
    std::ifstream in("graph.txt");
    if (in.is_open()) 
    {
        std::stringstream buffer;
        buffer << in.rdbuf();

        load(buffer);

        in.close();
    }
}

void Editor::load(std::stringstream& in)
{

    int countVert;
    in >> countVert;

    m_graph.clear();
    m_vertexCoords.clear();

    for (int i = 0; i < countVert; i++)
    {
        Vector2 coords;
        in >> coords.x >> coords.y;
        m_vertexCoords.push_back(coords);
        m_graph.addVertex();
    }
    for (int i = 0; i < m_graph.neighbours.size(); i++)
    {
        int parent;
        int countNeigh;
        char c;
        in >> parent >> countNeigh >> c;

        for (int j = 0; j < countNeigh; j++)
        {
            int neighbour;
            in >> neighbour;
            m_graph.addEdge(parent, neighbour);
        }
    }
}