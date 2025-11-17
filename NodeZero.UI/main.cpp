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

int main()
{
    const int screenWidth = static_cast<int>(GameConfig::DEFAULT_SCREEN_WIDTH);
    const int screenHeight = static_cast<int>(GameConfig::DEFAULT_SCREEN_HEIGHT);

    InitWindow(screenWidth, screenHeight, "NodeZero - Nodebuster Clone");
    SetExitKey(KEY_NULL);

    GameState currentState = GameState::MainMenu;
    GameState previousState = GameState::MainMenu;

    std::unique_ptr<IGame> game = GameFactory::CreateGame();
    game->Initialize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    auto eventLogger = std::make_shared<GameEventLogger>();
    game->GetEventManager().Subscribe("NodeSpawned", eventLogger);
    game->GetEventManager().Subscribe("NodeDestroyed", eventLogger);
    game->GetEventManager().Subscribe("NodeDamaged", eventLogger);

    std::unique_ptr<ICollisionSystem> collisionSystem = GameFactory::CreateCollisionSystem();
    std::unique_ptr<IScoreSystem> scoreSystem = GameFactory::CreateScoreSystem();

    const float damageZoneSize = 150.0f;
    const float damagePerSecond = 50.0f;

    float spawnTimer = 0.0f;

    std::unique_ptr<Menu> mainMenu = MenuFactory::CreateMainMenu(currentState);
    std::unique_ptr<Menu> pauseMenu = MenuFactory::CreatePauseMenu(currentState, *game, spawnTimer);
    const float spawnInterval = 2.0f;

    // CRT Shader setup
    RenderTexture2D renderTarget = LoadRenderTexture(screenWidth, screenHeight);

    // Fix texture wrapping pentru a preveni mirroring la margini
    SetTextureWrap(renderTarget.texture, TEXTURE_WRAP_CLAMP);

    // Debug: Log working directory
    TraceLog(LOG_INFO, "Working Directory: %s", GetWorkingDirectory());

    Shader crtShader = LoadShader(0, "assets/shaders/crt.fs");

    // Debug: Check if shader loaded successfully
    if (crtShader.id == 0)
    {
        TraceLog(LOG_ERROR, "====================================");
        TraceLog(LOG_ERROR, "SHADER FAILED TO LOAD!");
        TraceLog(LOG_ERROR, "Working dir: %s", GetWorkingDirectory());
        TraceLog(LOG_ERROR, "Looking for: assets/shaders/crt.fs");
        TraceLog(LOG_ERROR, "====================================");
    }
    else
    {
        TraceLog(LOG_INFO, "====================================");
        TraceLog(LOG_INFO, "CRT shader loaded successfully!");
        TraceLog(LOG_INFO, "Shader ID: %d", crtShader.id);
        TraceLog(LOG_INFO, "====================================");
    }

    // Get shader uniform locations
    int resolutionLoc = GetShaderLocation(crtShader, "resolution");
    int timeLoc = GetShaderLocation(crtShader, "time");

    TraceLog(LOG_INFO, "Shader uniform locations - resolution: %d, time: %d", resolutionLoc, timeLoc);

    // Set resolution uniform (static)
    float resolution[2] = {static_cast<float>(screenWidth), static_cast<float>(screenHeight)};
    SetShaderValue(crtShader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);

    float elapsedTime = 0.0f;

    SetTargetFPS(240);

    while (!WindowShouldClose() && currentState != GameState::Quit)
    {
        float deltaTime = GetFrameTime();
        elapsedTime += deltaTime;

        // Update shader time uniform
        SetShaderValue(crtShader, timeLoc, &elapsedTime, SHADER_UNIFORM_FLOAT);

        // Control cursor în funcție de stare
        if (currentState == GameState::Playing && previousState != GameState::Playing)
        {
            HideCursor(); // Ascunde cursorul când intri în Playing
        }
        else if (currentState != GameState::Playing && previousState == GameState::Playing)
        {
            ShowCursor(); // Arată cursorul când ieși din Playing
        }
        previousState = currentState;

        switch (currentState)
        {
        case GameState::MainMenu:
            mainMenu->Update();
            break;

        case GameState::Playing:
        {
            Vector2 mousePos = InputHandler::GetMousePosition();

            // Spawn noduri automat
            spawnTimer += deltaTime;
            if (spawnTimer >= spawnInterval)
            {
                spawnTimer = 0.0f;

                // Calculare centru ecran
                float centerX = screenWidth / 2.0f;
                float centerY = screenHeight / 2.0f;

                // Aleagere random a marginii (0=sus, 1=dreapta, 2=jos, 3=stânga)
                int edge = GetRandomValue(0, 3);
                float spawnX, spawnY;

                switch (edge)
                {
                case 0: // Sus
                    spawnX = static_cast<float>(GetRandomValue(50, screenWidth - 50));
                    spawnY = -50.0f;
                    break;
                case 1: // Dreapta
                    spawnX = static_cast<float>(screenWidth + 50);
                    spawnY = static_cast<float>(GetRandomValue(50, screenHeight - 50));
                    break;
                case 2: // Jos
                    spawnX = static_cast<float>(GetRandomValue(50, screenWidth - 50));
                    spawnY = static_cast<float>(screenHeight + 50);
                    break;
                case 3: // Stânga
                    spawnX = -50.0f;
                    spawnY = static_cast<float>(GetRandomValue(50, screenHeight - 50));
                    break;
                }

                game->SpawnNode(spawnX, spawnY);

                // Configurare direcție spre centru pentru nodul spawn-uit
                const auto &nodes = game->GetNodes();
                if (!nodes.empty())
                {
                    INode *lastNode = nodes.back();

                    // Punct țintă random în zona centrală (nu exact în centru)
                    // Offset random pentru a simula "gravity force" spre centru, nu convergență exactă
                    float offsetRange = 150.0f; // Raza zonei centrale
                    float targetX = centerX + static_cast<float>(GetRandomValue(-static_cast<int>(offsetRange), static_cast<int>(offsetRange)));
                    float targetY = centerY + static_cast<float>(GetRandomValue(-static_cast<int>(offsetRange), static_cast<int>(offsetRange)));

                    // Calculare vector direcție spre punctul țintă random
                    float dirX = targetX - spawnX;
                    float dirY = targetY - spawnY;

                    // Normalizare (convertire la vector unitar)
                    float length = sqrtf(dirX * dirX + dirY * dirY);
                    if (length > 0.0f)
                    {
                        dirX /= length;
                        dirY /= length;
                    }

                    // Setare direcție normalizată
                    lastNode->SetDirection(dirX, dirY);
                }
            }

            // Damage zone damage
            const auto &nodes = game->GetNodes();
            for (INode *node : nodes)
            {
                if (node->GetState() != NodeState::Active)
                    continue;

                float nodeX = node->GetPosition().x;
                float nodeY = node->GetPosition().y;
                float nodeSize = node->GetSize();

                // Verifică dacă nodul se intersectează cu zona de damage (pătrată)
                bool inDamageZone = collisionSystem->CheckRectCollision(
                    mousePos.x - damageZoneSize / 2, mousePos.y - damageZoneSize / 2, damageZoneSize, damageZoneSize,
                    nodeX - nodeSize, nodeY - nodeSize, nodeSize * 2, nodeSize * 2);

                if (inDamageZone)
                {
                    node->TakeDamage(damagePerSecond * deltaTime);
                }
            }

            game->Update(deltaTime);

            // Check for ESC to pause
            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentState = GameState::Paused;
            }
            break;
        }

        case GameState::Paused:
            pauseMenu->Update();
            // Check for ESC to resume game
            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentState = GameState::Playing;
            }
            break;

        case GameState::Settings: // Upgrades screen
            // Check for ESC to return to menu
            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentState = GameState::MainMenu;
            }
            break;

        default:
            break;
        }

        // Rendering pe RenderTexture (pentru shader CRT)
        BeginTextureMode(renderTarget);
        ClearBackground(Color{40, 40, 40, 255});

        // Draw based on game state
        switch (currentState)
        {
        case GameState::MainMenu:
            mainMenu->Draw();
            break;

        case GameState::Playing:
        {
            Vector2 mousePos = InputHandler::GetMousePosition();

            // Draw nodes with HP represented by fill
            const auto &nodes = game->GetNodes();
            for (const INode *node : nodes)
            {
                if (node->GetState() == NodeState::Active)
                {
                    float x = node->GetPosition().x;
                    float y = node->GetPosition().y;
                    float size = node->GetSize();
                    float hpPercentage = node->GetHP() / node->GetMaxHP();
                    float rotation = node->GetRotation();

                    switch (node->GetShape())
                    {
                    case NodeShape::Circle:
                        Renderer::DrawCircleNode(x, y, size, hpPercentage, BLUE);
                        break;
                    case NodeShape::Square:
                        Renderer::DrawSquareNode(x, y, size, hpPercentage, RED, rotation);
                        break;
                    case NodeShape::Hexagon:
                        Renderer::DrawHexagonNode(x, y, size, hpPercentage, DARKGREEN);
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

            // Draw damage zone DUPĂ noduri pentru blending (pătrat albastru semi-transparent)
            DrawRectangle(
                static_cast<int>(mousePos.x - damageZoneSize / 2),
                static_cast<int>(mousePos.y - damageZoneSize / 2),
                static_cast<int>(damageZoneSize),
                static_cast<int>(damageZoneSize),
                Color{0, 100, 255, 80});

            // Colțuri decorative în formă de L
            float cornerLength = 20.0f; // Lungimea fiecărei linii din L
            float cornerThickness = 3.0f;
            Color cornerColor = Color{0, 200, 255, 255}; // Cyan strălucitor

            float left = mousePos.x - damageZoneSize / 2;
            float right = mousePos.x + damageZoneSize / 2;
            float top = mousePos.y - damageZoneSize / 2;
            float bottom = mousePos.y + damageZoneSize / 2;

            // Colț stânga-sus
            DrawLineEx(Vector2{left, top}, Vector2{left + cornerLength, top}, cornerThickness, cornerColor);
            DrawLineEx(Vector2{left, top}, Vector2{left, top + cornerLength}, cornerThickness, cornerColor);

            // Colț dreapta-sus
            DrawLineEx(Vector2{right, top}, Vector2{right - cornerLength, top}, cornerThickness, cornerColor);
            DrawLineEx(Vector2{right, top}, Vector2{right, top + cornerLength}, cornerThickness, cornerColor);

            // Colț stânga-jos
            DrawLineEx(Vector2{left, bottom}, Vector2{left + cornerLength, bottom}, cornerThickness, cornerColor);
            DrawLineEx(Vector2{left, bottom}, Vector2{left, bottom - cornerLength}, cornerThickness, cornerColor);

            // Colț dreapta-jos
            DrawLineEx(Vector2{right, bottom}, Vector2{right - cornerLength, bottom}, cornerThickness, cornerColor);
            DrawLineEx(Vector2{right, bottom}, Vector2{right, bottom - cornerLength}, cornerThickness, cornerColor);

            // Pătrat alb în centru (cursor indicator)
            float centerSquareSize = 8.0f;
            DrawRectangle(
                static_cast<int>(mousePos.x - centerSquareSize / 2),
                static_cast<int>(mousePos.y - centerSquareSize / 2),
                static_cast<int>(centerSquareSize),
                static_cast<int>(centerSquareSize),
                WHITE);

            // Draw UI
            UI::DrawTitle("NodeZero - Nodebuster Clone", 10, 10, 20, DARKGRAY);
            UI::DrawDebugInfo(10, 40);
            break;
        }

        case GameState::Paused:
        {
            Vector2 mousePos = InputHandler::GetMousePosition();

            const auto &nodes = game->GetNodes();
            for (const INode *node : nodes)
            {
                if (node->GetState() == NodeState::Active)
                {
                    float x = node->GetPosition().x;
                    float y = node->GetPosition().y;
                    float size = node->GetSize();
                    float hpPercentage = node->GetHP() / node->GetMaxHP();
                    float rotation = node->GetRotation();

                    switch (node->GetShape())
                    {
                    case NodeShape::Circle:
                        Renderer::DrawCircleNode(x, y, size, hpPercentage, BLUE);
                        break;
                    case NodeShape::Square:
                        Renderer::DrawSquareNode(x, y, size, hpPercentage, RED, rotation);
                        break;
                    case NodeShape::Hexagon:
                        Renderer::DrawHexagonNode(x, y, size, hpPercentage, DARKGREEN);
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

            // Draw damage zone DUPĂ noduri pentru blending (pătrat albastru semi-transparent)
            DrawRectangle(
                static_cast<int>(mousePos.x - damageZoneSize / 2),
                static_cast<int>(mousePos.y - damageZoneSize / 2),
                static_cast<int>(damageZoneSize),
                static_cast<int>(damageZoneSize),
                Color{0, 100, 255, 80});

            // Colțuri decorative în formă de L
            float cornerLength = 20.0f; // Lungimea fiecărei linii din L
            float cornerThickness = 3.0f;
            Color cornerColor = Color{0, 200, 255, 255}; // Cyan strălucitor

            float left = mousePos.x - damageZoneSize / 2;
            float right = mousePos.x + damageZoneSize / 2;
            float top = mousePos.y - damageZoneSize / 2;
            float bottom = mousePos.y + damageZoneSize / 2;

            // Colț stânga-sus
            DrawLineEx(Vector2{left, top}, Vector2{left + cornerLength, top}, cornerThickness, cornerColor);
            DrawLineEx(Vector2{left, top}, Vector2{left, top + cornerLength}, cornerThickness, cornerColor);

            // Colț dreapta-sus
            DrawLineEx(Vector2{right, top}, Vector2{right - cornerLength, top}, cornerThickness, cornerColor);
            DrawLineEx(Vector2{right, top}, Vector2{right, top + cornerLength}, cornerThickness, cornerColor);

            // Colț stânga-jos
            DrawLineEx(Vector2{left, bottom}, Vector2{left + cornerLength, bottom}, cornerThickness, cornerColor);
            DrawLineEx(Vector2{left, bottom}, Vector2{left, bottom - cornerLength}, cornerThickness, cornerColor);

            // Colț dreapta-jos
            DrawLineEx(Vector2{right, bottom}, Vector2{right - cornerLength, bottom}, cornerThickness, cornerColor);
            DrawLineEx(Vector2{right, bottom}, Vector2{right, bottom - cornerLength}, cornerThickness, cornerColor);

            // Pătrat alb în centru (cursor indicator)
            float centerSquareSize = 8.0f;
            DrawRectangle(
                static_cast<int>(mousePos.x - centerSquareSize / 2),
                static_cast<int>(mousePos.y - centerSquareSize / 2),
                static_cast<int>(centerSquareSize),
                static_cast<int>(centerSquareSize),
                WHITE);

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

        EndTextureMode();

        // Desenare pe ecran cu shader CRT
        BeginDrawing();
        ClearBackground(BLACK);

        BeginShaderMode(crtShader);
        // Desenare texture (flip vertical pentru Raylib)
        DrawTextureRec(
            renderTarget.texture,
            Rectangle{0, 0, static_cast<float>(renderTarget.texture.width), static_cast<float>(-renderTarget.texture.height)},
            Vector2{0, 0},
            WHITE);
        EndShaderMode();

        EndDrawing();
    }

    // Cleanup
    UnloadShader(crtShader);
    UnloadRenderTexture(renderTarget);

    ShowCursor(); // Asigură-te că cursorul e vizibil la închidere
    CloseWindow();

    return 0;
}
