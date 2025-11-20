#include "../include/GameApp.h"

#include <algorithm>
#include <iostream>

#include "../include/GameEventLogger.h"
#include "../include/InputHandler.h"
#include "../include/Renderer.h"
#include "../include/States/GameplayState.h"
#include "../include/States/MainMenuState.h"
#include "../include/States/PauseMenuState.h"
#include "../include/States/SettingsState.h"
#include "../include/UI.h"
#include "Config/GameConfig.h"
#include "Game.h"
#include "IGame.h"
#include "raymath.h"

GameApp::GameApp()
    : m_CurrentState(GameState::MainMenu),
      m_PreviousState(GameState::MainMenu),
      m_ShouldClose(false),
      m_ElapsedTime(0.0f),
      m_ResolutionLoc(0),
      m_TimeLoc(0) {
}

GameApp::~GameApp() {
    Cleanup();
}

void GameApp::Initialize() {
    const int screenWidth = static_cast<int>(GameConfig::DEFAULT_SCREEN_WIDTH);
    const int screenHeight = static_cast<int>(GameConfig::DEFAULT_SCREEN_HEIGHT);

    InitWindow(screenWidth, screenHeight, "NodeZero - Nodebuster Clone");
    SetExitKey(KEY_NULL);
    SetTargetFPS(240);

    // Initialize Game
    m_Game = std::make_unique<Game>();
    m_Game->Initialize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    // Event Logger
    auto eventLogger = std::make_shared<GameEventLogger>();
    m_Game->GetEventManager().Subscribe("NodeSpawned", eventLogger);
    m_Game->GetEventManager().Subscribe("NodeDestroyed", eventLogger);
    m_Game->GetEventManager().Subscribe("NodeDamaged", eventLogger);

    // Initialize States
    auto stateChangeCallback = [this](GameState newState) { ChangeState(newState); };

    m_States[GameState::MainMenu] = std::make_unique<MainMenuState>(stateChangeCallback);
    m_States[GameState::Playing] = std::make_unique<GameplayState>(*m_Game, stateChangeCallback);
    m_States[GameState::Paused] = std::make_unique<PauseMenuState>(*m_Game, stateChangeCallback);
    m_States[GameState::Settings] = std::make_unique<SettingsState>(*m_Game, stateChangeCallback);

    // CRT Shader setup
    m_RenderTarget = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureWrap(m_RenderTarget.texture, TEXTURE_WRAP_CLAMP);

    m_CrtShader = LoadShader(0, "assets/shaders/crt.fs");

    m_ResolutionLoc = GetShaderLocation(m_CrtShader, "resolution");
    m_TimeLoc = GetShaderLocation(m_CrtShader, "time");

    float resolution[2] = {static_cast<float>(screenWidth), static_cast<float>(screenHeight)};
    SetShaderValue(m_CrtShader, m_ResolutionLoc, resolution, SHADER_UNIFORM_VEC2);
}

void GameApp::ChangeState(GameState newState) {
    m_CurrentState = newState;
}

void GameApp::Run() {
    Initialize();

    while (!WindowShouldClose() && m_CurrentState != GameState::Quit) {
        Update();
        Draw();
    }
}

void GameApp::Update() {
    float deltaTime = GetFrameTime();
    m_ElapsedTime += deltaTime;

    SetShaderValue(m_CrtShader, m_TimeLoc, &m_ElapsedTime, SHADER_UNIFORM_FLOAT);

    if (m_CurrentState == GameState::Playing && m_PreviousState != GameState::Playing) {
        HideCursor();
    } else if (m_CurrentState != GameState::Playing && m_PreviousState == GameState::Playing) {
        ShowCursor();
    }
    m_PreviousState = m_CurrentState;

    if (m_States.find(m_CurrentState) != m_States.end()) {
        m_States[m_CurrentState]->Update(deltaTime);
    }
}

void GameApp::Draw() {
    BeginTextureMode(m_RenderTarget);
    ClearBackground(Color{40, 40, 40, 255});

    // Always draw gameplay if playing or paused (so pause menu has game background)
    if (m_CurrentState == GameState::Playing || m_CurrentState == GameState::Paused) {
        if (m_States.find(GameState::Playing) != m_States.end()) {
            m_States[GameState::Playing]->Draw();
        }
    }

    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

    BeginShaderMode(m_CrtShader);
    DrawTextureRec(
        m_RenderTarget.texture,
        Rectangle{0, 0, static_cast<float>(m_RenderTarget.texture.width), static_cast<float>(-m_RenderTarget.texture.height)},
        Vector2{0, 0},
        WHITE);
    EndShaderMode();

    // Draw UI on top of shader
    if (m_CurrentState == GameState::Playing) {
        UI::DrawTitle("NodeZero - Nodebuster Clone", 10, 10, 20, DARKGRAY);
        UI::DrawDebugInfo(10, 40);
        UI::DrawScore(m_Game->GetPickupScore(), 10, 70, 20, WHITE);
        UI::DrawHealthBar(m_Game->GetCurrentHealth(), m_Game->GetMaxHealth(), 10, 100, 200, 24);
    } else if (m_CurrentState == GameState::Paused) {
        UI::DrawScore(m_Game->GetPickupScore(), 10, 70, 20, WHITE);
        if (m_States.find(GameState::Paused) != m_States.end()) {
            m_States[GameState::Paused]->Draw();
        }
    } else {
        if (m_States.find(m_CurrentState) != m_States.end()) {
            m_States[m_CurrentState]->Draw();
        }
    }

    EndDrawing();
}

void GameApp::Cleanup() {
    UnloadShader(m_CrtShader);
    UnloadRenderTexture(m_RenderTarget);
    ShowCursor();
    CloseWindow();
}
