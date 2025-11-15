#include <cmath>
#include <vector>

#include "../NodeZero.Core/GameAPI.h"
#include "../NodeZero.Core/Entities/Bullet.h"
#include "Rendering/PlayerRenderer.h"
#include "Rendering/BulletRenderer.h"
#include "UI/HUD.h"
#include "raylib.h"
#include "raymath.h"

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "NodeZero - Arrow Character");

    IGame* game = GameFactory::CreateGame();
    game->Initialize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    const float centerX = screenWidth / 2.0f;
    const float centerY = screenHeight / 2.0f;

    std::vector<Bullet> bullets;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        Vector2 mousePos = GetMousePosition();

        float deltaX = mousePos.x - centerX;
        float deltaY = mousePos.y - centerY;
        float angleRadians = atan2f(deltaY, deltaX);
        float angleDegrees = angleRadians * RAD2DEG;

        float adjustedAngle = angleDegrees + 90.0f;

        IPlayer& player = game->GetPlayer();
        player.SetRotationDegrees(adjustedAngle);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            bullets.emplace_back(centerX, centerY, adjustedAngle);
        }

        for (auto& bullet : bullets) {
            bullet.Update(deltaTime);
        }

        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(), 
                [screenWidth, screenHeight](const Bullet& b) {
                    return b.IsOutOfBounds(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
                }),
            bullets.end()
        );

        game->Update(deltaTime);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        PlayerRenderer::DrawArrow(centerX, centerY, 30.0f, adjustedAngle, MAROON);

        for (const auto& bullet : bullets) {
            BulletRenderer::DrawBullet(bullet.GetX(), bullet.GetY(), 8.0f, bullet.GetRotationDegrees(), RED);
        }

        HUD::DrawTitle("Desenata sageata", 10, 10, 20, DARKGRAY);
        HUD::DrawDebugInfo(10, 40);

        EndDrawing();
    }

    GameFactory::DestroyGame(game);
    CloseWindow();

    return 0;
}