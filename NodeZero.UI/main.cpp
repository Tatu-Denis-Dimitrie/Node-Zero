#include <algorithm>
#include <memory>
#include <vector>

#include "Config/GameConfig.h"
#include "GameFactory.h"
#include "IGame.h"
#include "INode.h"
#include "Systems/ICollisionSystem.h"
#include "Systems/IScoreSystem.h"
#include "Events/EventManager.h"
#include "include/InputHandler.h"
#include "include/Renderer.h"
#include "include/UI.h"
#include "include/GameEventLogger.h"
#include "raylib.h"

int main() {
    const int screenWidth = static_cast<int>(GameConfig::DEFAULT_SCREEN_WIDTH);
    const int screenHeight = static_cast<int>(GameConfig::DEFAULT_SCREEN_HEIGHT);

    InitWindow(screenWidth, screenHeight, "NodeZero - Nodebuster Clone");

    std::unique_ptr<IGame> game = GameFactory::CreateGame();
    game->Initialize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    // Setup Event System (Observer Pattern)
    auto eventLogger = std::make_shared<NodeZero::GameEventLogger>();
    game->GetEventManager().Subscribe("NodeSpawned", eventLogger);
    game->GetEventManager().Subscribe("NodeDestroyed", eventLogger);
    game->GetEventManager().Subscribe("NodeDamaged", eventLogger);

    std::unique_ptr<ICollisionSystem> collisionSystem = GameFactory::CreateCollisionSystem();
    std::unique_ptr<IScoreSystem> scoreSystem = GameFactory::CreateScoreSystem();

    // Damage zone configuration
    const float damageZoneSize = 100.0f;
    const float damagePerSecond = 50.0f;

    // Node spawner configuration
    float spawnTimer = 0.0f;
    const float spawnInterval = 2.0f;

    SetTargetFPS(240);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        Vector2 mousePos = InputHandler::GetMousePosition();

        // Spawn noduri automat
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;

            float spawnY = static_cast<float>(GetRandomValue(50, screenHeight - 50));
            float spawnX = static_cast<float>(screenWidth + 50);

            NodeShape randomShape = static_cast<NodeShape>(GetRandomValue(0, 2));  // Circle, Square, Triangle
            float randomSize = static_cast<float>(GetRandomValue(20, 50));
            float randomSpeed = static_cast<float>(GetRandomValue(50, 150));

            game->SpawnNode(spawnX, spawnY);

            // Configurare nod spawn-uit
            const auto& nodes = game->GetNodes();
            if (!nodes.empty()) {
                INode* lastNode = nodes.back();
            }
        }

        // Damage zone damage
        const auto& nodes = game->GetNodes();
        for (INode* node : nodes) {
            if (node->GetState() != NodeState::Active)
                continue;

            float nodeX = node->GetPosition().GetX();
            float nodeY = node->GetPosition().GetY();
            float nodeSize = node->GetSize();

            // Verifică dacă nodul se intersectează cu zona de damage (pătrată)
            bool inDamageZone = collisionSystem->CheckRectCollision(
                mousePos.x - damageZoneSize / 2, mousePos.y - damageZoneSize / 2, damageZoneSize, damageZoneSize,
                nodeX - nodeSize, nodeY - nodeSize, nodeSize * 2, nodeSize * 2);

            if (inDamageZone) {
                node->TakeDamage(damagePerSecond * deltaTime);
            }
        }

        game->Update(deltaTime);

        // Rendering
        BeginDrawing();
        ClearBackground(Color{40, 40, 40, 255});

        // Draw damage zone (pătrat roșu semi-transparent în jurul mouse-ului)
        DrawRectangle(
            static_cast<int>(mousePos.x - damageZoneSize / 2),
            static_cast<int>(mousePos.y - damageZoneSize / 2),
            static_cast<int>(damageZoneSize),
            static_cast<int>(damageZoneSize),
            Color{255, 0, 0, 50});
        DrawRectangleLines(
            static_cast<int>(mousePos.x - damageZoneSize / 2),
            static_cast<int>(mousePos.y - damageZoneSize / 2),
            static_cast<int>(damageZoneSize),
            static_cast<int>(damageZoneSize),
            RED);

        // Draw nodes with HP represented by fill
        for (const INode* node : nodes) {
            if (node->GetState() == NodeState::Active) {
                float x = node->GetPosition().GetX();
                float y = node->GetPosition().GetY();
                float size = node->GetSize();
                float hpPercentage = node->GetHP() / node->GetMaxHP();

                switch (node->GetShape()) {
                    case NodeShape::Circle:
                        Renderer::DrawCircleNode(x, y, size, hpPercentage, BLUE);
                        break;
                    case NodeShape::Square:
                        Renderer::DrawSquareNode(x, y, size, hpPercentage, GREEN);
                        break;
                    case NodeShape::Triangle:
                        Renderer::DrawTriangleNode(x, y, size, hpPercentage, ORANGE);
                        break;
                    default:
                        Renderer::DrawCircleNode(x, y, size, hpPercentage, PURPLE);
                        break;
                }
            }
        }

        // Draw UI
        UI::DrawTitle("NodeZero - Nodebuster Clone", 10, 10, 20, DARKGRAY);
        UI::DrawDebugInfo(10, 40);

        EndDrawing();
    }

    // Cleanup automat prin unique_ptr
    CloseWindow();

    return 0;
}
