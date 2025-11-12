#include "raylib.h"

int main()
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "NodeZero - Raylib Test");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        
        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congratulations! Raylib is working!", 190, 200, 20, LIGHTGRAY);
            
            DrawCircle(screenWidth/2, screenHeight/2 + 50, 50, MAROON);
            
            DrawFPS(10, 10);

        EndDrawing();
    }

    // De-initialization
    CloseWindow();

    return 0;
}