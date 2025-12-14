#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <vector>
#include <string>
#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)

#define WINDOW_TITLE "Window title"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(120);

    //Texture2D texture = LoadTexture(ASSETS_PATH"test.png"); // Check README.md for how this works
    bool showMessageBox = false;
    std::vector<Vector2> ballCenters;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        Vector2 mousePosition = GetMousePosition();
        //DrawTexture(texture, texture_x, texture_y, WHITE);
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) == true)
        {
            Vector2 centerballPosition = GetMousePosition();
            ballCenters.push_back(centerballPosition);
            centerballPosition.x = GetMouseX();
            centerballPosition.y = GetMouseY();
            
            DrawCircleV(centerballPosition, 50, BEIGE);
        }
        for (Vector2 ballCenter : ballCenters) 
        {
            float radius = 50;
            Color color = BEIGE;
            if (Vector2Distance(ballCenter, mousePosition) <= radius)
            {
                color = BROWN;
            }
            DrawCircleV(ballCenter, 50, color);
        }
        const Vector2 text_size = MeasureTextEx(GetFontDefault(), " ", 20, 1);
        std::string text = std::to_string(mousePosition.x) + " " + std::to_string(mousePosition.y);
        DrawText(text.c_str(), 3, 3, 20, BLACK);

        for (int i = 0; i < ballCenters.size(); i++) 
        {
            Vector2 ballCenter = ballCenters[i];
            std::string text = std::to_string(ballCenter.x) + " " + std::to_string(ballCenter.y);
            DrawText(text.c_str(), 3, 3 + text_size.y * (i + 1), 20, BLACK);
        }

        if (GuiButton(Rectangle{ 24, 24, 120, 30 }, "#191#Show Message")) showMessageBox = true;

        if (showMessageBox)
        {
            int result = GuiMessageBox(Rectangle{ 85, 70, 250, 100 },
                "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

            if (result >= 0) showMessageBox = false;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
