#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "editor.hpp"

#include <vector>
#include <string>

#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)
#define WINDOW_TITLE "Window title"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetExitKey(0);
    SetTargetFPS(120);

    //Texture2D texture = LoadTexture(ASSETS_PATH"test.png"); // Check README.md for how this works
    
    Editor editor;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        editor.tick();
        
        Vector2 mousePosition = GetMousePosition();
        //DrawTexture(texture, texture_x, texture_y, WHITE);
        std::string text = std::to_string(mousePosition.x) + " " + std::to_string(mousePosition.y);
        DrawText(text.c_str(), 3, 3, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
