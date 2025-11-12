#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include "../GameAPI/GameAPI.h"  
#include "Rendering/PlayerRenderer.h"
#include "UI/HUD.h"

int main()
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "NodeZero - Arrow Character");
    ToggleFullscreen();  

    IGame* game = GameFactory::CreateGame();
    game->Initialize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    const float centerX = screenWidth / 2.0f;
    const float centerY = screenHeight / 2.0f;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        
        Vector2 mousePos = GetMousePosition();
        
        float deltaX = mousePos.x - centerX;
        float deltaY = mousePos.y - centerY;
        float angleRadians = atan2f(deltaY, deltaX);
        float angleDegrees = angleRadians * RAD2DEG;
        
        float adjustedAngle = angleDegrees + 90.0f;
        
        IPlayer& player = game->GetPlayer();
        player.SetRotationDegrees(adjustedAngle);
        
        game->Update(deltaTime);
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            PlayerRenderer::DrawArrow(centerX, centerY, 30.0f, adjustedAngle, MAROON);
            
            HUD::DrawTitle("Desenata sageata", 10, 10, 20, DARKGRAY);
            HUD::DrawDebugInfo(10, 40);

        EndDrawing();
    }

    GameFactory::DestroyGame(game);
    CloseWindow();

    return 0;
}