#include "raylib.h"

#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)

#define WINDOW_TITLE "Window title"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(120);

    //Texture2D texture = LoadTexture(ASSETS_PATH"test.png"); // Check README.md for how this works
    while (!WindowShouldClose())
    {
        BeginDrawing();
        //ClearBackground(RAYWHITE);
        //const int texture_x = SCREEN_WIDTH / 2 - texture.width / 2;
        //const int texture_y = SCREEN_HEIGHT / 2 - texture.height / 2;

        Vector2 mousePosition = GetMousePosition();
        
        //DrawTexture(texture, texture_x, texture_y, WHITE);
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) == true)
        {
            Vector2 centerballPosition;
            centerballPosition.x = GetMouseX();
            centerballPosition.y = GetMouseY();
            
            DrawCircleV(centerballPosition, 50, BEIGE);
        }
        if (IsKeyReleased(KEY_C) == true) 
        {
            ClearBackground(RAYWHITE);
        }

        //const char* text = "OMG! IT WORKS!";
        //const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
        //DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, texture_y + texture.height + text_size.y + 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
