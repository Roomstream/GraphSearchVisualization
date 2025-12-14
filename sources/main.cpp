#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <vector>
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
        //const int texture_x = SCREEN_WIDTH / 2 - texture.width / 2;
        //const int texture_y = SCREEN_HEIGHT / 2 - texture.height / 2;

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
            DrawCircleV(ballCenter, 50, BEIGE);
        }
        
        if (GuiButton(Rectangle{ 24, 24, 120, 30 }, "#191#Show Message")) showMessageBox = true;

        if (showMessageBox)
        {
            int result = GuiMessageBox(Rectangle{ 85, 70, 250, 100 },
                "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

            if (result >= 0) showMessageBox = false;
        }

        //const char* text = "OMG! IT WORKS!";
        //const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
        //DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, texture_y + texture.height + text_size.y + 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
