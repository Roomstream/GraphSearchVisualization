#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "editor.hpp"

#include <vector>
#include <string>

#define SCREEN_WIDTH (1366)
#define SCREEN_HEIGHT (768)
#define WINDOW_TITLE "Graph Demo"

int main(void)
{
    // Изменение размера окна некорректно работает при открытии страницы на компьютере
    // Поэтому выключаем в браузерной компьютерной версии 
#if not(__EMSCRIPTEN__) || TARGET_PHONE
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
#endif
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
