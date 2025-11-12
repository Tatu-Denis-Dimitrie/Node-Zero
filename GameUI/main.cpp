#include "raylib.h"
#include "../GameAPI/Entities/Game.h"
#include "Rendering/PlayerRenderer.h"
#include "UI/HUD.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "NodeZero - Arrow Character");

    Game game;
    game.Initialize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    const float centerX = screenWidth / 2.0f;
    const float centerY = screenHeight / 2.0f;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        game.Update(deltaTime);
        
        IPlayer& player = game.GetPlayer();
        float playerRotation = static_cast<float>(player.GetRotation());
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            PlayerRenderer::DrawArrow(centerX, centerY, 30.0f, playerRotation, MAROON);
            
            HUD::DrawTitle("Triughi desenat", 10, 10, 20, DARKGRAY);
            HUD::DrawDebugInfo(10, 40);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}