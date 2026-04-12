#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "editor.hpp"

#include <vector>
#include <string>

#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)
#define WINDOW_TITLE "Graph Demo"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetExitKey(0);
    SetTargetFPS(120);

    Editor editor;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        editor.tick();
        
        Vector2 mousePosition = GetMousePosition();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
