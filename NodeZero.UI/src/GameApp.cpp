#include "GameApp.h"

#include <algorithm>
#include <iostream>

#include "Config/GameConfig.h"
#include "EventLogger.h"
#include "Game.h"
#include "IGame.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "Screens/GameoverScreen.h"
#include "Screens/GameplayScreen.h"
#include "Screens/LevelCompletedScreen.h"
#include "Screens/MainScreen.h"
#include "Screens/PauseScreen.h"
#include "Screens/UpgradesScreen.h"
#include "raymath.h"

GameApp::GameApp()
    : m_CurrentState(GameScreen::MainMenu),
      m_PreviousState(GameScreen::MainMenu),
      m_ShouldClose(false),
      m_ElapsedTime(0.0f),
      m_ResolutionLoc(0),
      m_TimeLoc(0) {
}

GameApp::~GameApp() {
    Cleanup();
}

void GameApp::Initialize() {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(0, 0, "NodeZero");

    int monitor = GetCurrentMonitor();
    int width = GetMonitorWidth(monitor);
    int height = GetMonitorHeight(monitor);

    SetWindowSize(width, height);
    SetWindowPosition(0, 0);

    SetExitKey(KEY_NULL);
    SetTargetFPS(240);

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Load custom font
    m_Font = LoadFont("assets/fonts/ari-w9500-display.ttf");

    // Initialize Game
    m_Game = std::make_unique<Game>();
    m_Game->Initialize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    // Observer Logger
    auto eventLogger = std::make_shared<EventLogger>();
    m_Game->Attach(eventLogger);

    // Initialize Screens
    auto stateChangeCallback = [this](GameScreen newState) { ChangeState(newState); };

    m_MainScreen = std::make_unique<MainScreen>(stateChangeCallback, m_Font);
    m_GameplayScreen = std::make_shared<GameplayScreen>(*m_Game, stateChangeCallback, m_Font);
    m_PauseScreen = std::make_unique<PauseScreen>(*m_Game, stateChangeCallback, m_Font);
    m_UpgradesScreen = std::make_unique<UpgradesScreen>(*m_Game, stateChangeCallback, m_Font);
    m_LevelCompletedScreen = std::make_unique<LevelCompletedScreen>(*m_Game, stateChangeCallback, m_Font);
    m_GameoverScreen = std::make_unique<GameoverScreen>(*m_Game, stateChangeCallback, m_Font);

    // Attach GameplayScreen as observer to receive game events
    m_Game->Attach(m_GameplayScreen);

    // CRT Shader setup
    m_RenderTarget = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureWrap(m_RenderTarget.texture, TEXTURE_WRAP_CLAMP);

    m_CrtShader = LoadShader(0, "assets/shaders/crt.fs");

    m_ResolutionLoc = GetShaderLocation(m_CrtShader, "resolution");
    m_TimeLoc = GetShaderLocation(m_CrtShader, "time");

    float resolution[2] = {static_cast<float>(screenWidth), static_cast<float>(screenHeight)};
    SetShaderValue(m_CrtShader, m_ResolutionLoc, resolution, SHADER_UNIFORM_VEC2);
}

void GameApp::ChangeState(GameScreen newState) {
    m_CurrentState = newState;
}

void GameApp::Run() {
    Initialize();

    while (!WindowShouldClose() && m_CurrentState != GameScreen::Quit) {
        Update();
        Draw();
    }
}

void GameApp::Update() {
    float deltaTime = GetFrameTime();
    m_ElapsedTime += deltaTime;

    SetShaderValue(m_CrtShader, m_TimeLoc, &m_ElapsedTime, SHADER_UNIFORM_FLOAT);

    if (m_CurrentState == GameScreen::Playing && m_PreviousState != GameScreen::Playing) {
        HideCursor();
        if (m_PreviousState == GameScreen::GameOver || m_PreviousState == GameScreen::MainMenu) {
            m_GameplayScreen->ClearEffects();
        }
    } else if (m_CurrentState != GameScreen::Playing && m_PreviousState == GameScreen::Playing) {
        ShowCursor();
    }
    m_PreviousState = m_CurrentState;

    switch (m_CurrentState) {
        case GameScreen::MainMenu:
            m_MainScreen->Update(deltaTime);
            break;
        case GameScreen::Playing:
            m_GameplayScreen->Update(deltaTime);
            break;
        case GameScreen::Paused:
            m_PauseScreen->Update(deltaTime);
            break;
        case GameScreen::LevelCompleted:
            m_LevelCompletedScreen->Update(deltaTime);
            break;
        case GameScreen::Settings:
            m_UpgradesScreen->Update(deltaTime);
            break;
        case GameScreen::GameOver:
            m_GameoverScreen->Update(deltaTime);
            break;
        case GameScreen::Quit:
            break;
    }
}

void GameApp::Draw() {
    // Draw EVERYTHING to the render target first (so CRT shader applies to all)
    BeginTextureMode(m_RenderTarget);
    ClearBackground(Color{40, 40, 40, 255});

    // Always draw gameplay if playing or paused (so pause menu has game background)
    if (m_CurrentState == GameScreen::Playing || m_CurrentState == GameScreen::Paused || m_CurrentState == GameScreen::LevelCompleted || m_CurrentState == GameScreen::GameOver) {
        m_GameplayScreen->Draw();
    }

    // Draw all UI elements inside the render target (before shader)
    switch (m_CurrentState) {
        case GameScreen::Playing:
            // HUD is now drawn inside GameplayScreen::Draw() to be affected by camera shake
            break;
        case GameScreen::Paused:
            m_PauseScreen->Draw();
            break;
        case GameScreen::LevelCompleted:
            m_LevelCompletedScreen->Draw();
            break;
        case GameScreen::MainMenu:
            m_MainScreen->Draw();
            break;
        case GameScreen::Settings:
            m_UpgradesScreen->Draw();
            break;
        case GameScreen::GameOver:
            m_GameoverScreen->Draw();
            break;
        case GameScreen::Quit:
            break;
    }

    EndTextureMode();

    // Now draw the render target with CRT shader applied
    BeginDrawing();
    ClearBackground(BLACK);

    BeginShaderMode(m_CrtShader);
    DrawTextureRec(
        m_RenderTarget.texture,
        Rectangle{0, 0, static_cast<float>(m_RenderTarget.texture.width), static_cast<float>(-m_RenderTarget.texture.height)},
        Vector2{0, 0},
        WHITE);
    EndShaderMode();

    EndDrawing();
}

void GameApp::Cleanup() {
    UnloadFont(m_Font);
    UnloadShader(m_CrtShader);
    UnloadRenderTexture(m_RenderTarget);
    ShowCursor();
    CloseWindow();
}
