#include <algorithm>
#include <memory>
#include <vector>

#include "Config/GameConfig.h"
#include "Enums/GameState.h"
#include "Events/EventManager.h"
#include "GameFactory.h"
#include "IGame.h"
#include "INode.h"
#include "Systems/ICollisionSystem.h"
#include "Systems/IScoreSystem.h"
#include "include/GameEventLogger.h"
#include "include/InputHandler.h"
#include "include/MenuFactory.h"
#include "include/Renderer.h"
#include "include/UI.h"
#include "raylib.h"

int main() {
    const int screenWidth = static_cast<int>(GameConfig::DEFAULT_SCREEN_WIDTH);
    const int screenHeight = static_cast<int>(GameConfig::DEFAULT_SCREEN_HEIGHT);

    InitWindow(screenWidth, screenHeight, "NodeZero - Nodebuster Clone");
    SetExitKey(KEY_NULL);

    GameState currentState = GameState::MainMenu;

    std::unique_ptr<Menu> mainMenu = MenuFactory::CreateMainMenu(currentState);
    std::unique_ptr<Menu> pauseMenu = MenuFactory::CreatePauseMenu(currentState);

    std::unique_ptr<IGame> game = GameFactory::CreateGame();
    game->Initialize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    auto eventLogger = std::make_shared<GameEventLogger>();
    game->GetEventManager().Subscribe("NodeSpawned", eventLogger);
    game->GetEventManager().Subscribe("NodeDestroyed", eventLogger);
    game->GetEventManager().Subscribe("NodeDamaged", eventLogger);

    std::unique_ptr<ICollisionSystem> collisionSystem = GameFactory::CreateCollisionSystem();
    std::unique_ptr<IScoreSystem> scoreSystem = GameFactory::CreateScoreSystem();

    const float damageZoneSize = 100.0f;
    const float damagePerSecond = 50.0f;

    float spawnTimer = 0.0f;
    const float spawnInterval = 2.0f;

    SetTargetFPS(240);

    while (!WindowShouldClose() && currentState != GameState::Quit) {
        float deltaTime = GetFrameTime();

        // Update based on game state
        switch (currentState) {
            case GameState::MainMenu:
                mainMenu->Update();
                break;

            case GameState::Playing: {
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

                    float nodeX = node->GetPosition().x;
                    float nodeY = node->GetPosition().y;
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

                // Check for ESC to pause
                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentState = GameState::Paused;
                }
                break;
            }

            case GameState::Paused:
                pauseMenu->Update();
                // Check for ESC to resume game
                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentState = GameState::Playing;
                }
                break;

            case GameState::Settings: // Upgrades screen
                // Check for ESC to return to menu
                if (IsKeyPressed(KEY_ESCAPE)) {
                    currentState = GameState::MainMenu;
                }
                break;

            default:
                break;
        }

        // Rendering
        BeginDrawing();
        ClearBackground(Color{40, 40, 40, 255});

        // Draw based on game state
        switch (currentState) {
            case GameState::MainMenu:
                mainMenu->Draw();
                break;

            case GameState::Playing: {
                Vector2 mousePos = InputHandler::GetMousePosition();

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
                const auto& nodes = game->GetNodes();
                for (const INode* node : nodes) {
                    if (node->GetState() == NodeState::Active) {
                        float x = node->GetPosition().x;
                        float y = node->GetPosition().y;
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
                break;
            }

            case GameState::Paused: {
                Vector2 mousePos = InputHandler::GetMousePosition();

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

                const auto& nodes = game->GetNodes();
                for (const INode* node : nodes) {
                    if (node->GetState() == NodeState::Active) {
                        float x = node->GetPosition().x;
                        float y = node->GetPosition().y;
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

                DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 150});

                pauseMenu->Draw();
                break;
            }

            case GameState::Settings: 
                DrawText("UPGRADES", screenWidth / 2 - 100, screenHeight / 2 - 150, 40, WHITE);
                DrawText("Coming Soon...", screenWidth / 2 - 100, screenHeight / 2 - 50, 30, GRAY);
                DrawText("Press ESC to return to menu", screenWidth / 2 - 180, screenHeight / 2 + 50, 20, LIGHTGRAY);
                break;

            default:
                break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
