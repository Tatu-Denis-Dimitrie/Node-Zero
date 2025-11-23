#pragma once

#include <memory>

#include "Enums/GameScreen.h"
#include "raylib.h"

class IGame;
class GameplayScreen;
class MainMenuScreen;
class PauseMenuScreen;
class SettingsScreen;
class LevelCompletedScreen;
class GameOverScreen;

class GameApp {
   public:
    GameApp();
    ~GameApp();

    void Run();

   private:
    void Initialize();
    void Update();
    void Draw();
    void Cleanup();

    void ChangeState(GameScreen newState);

    // Game Screen
    GameScreen m_CurrentState;
    GameScreen m_PreviousState;
    bool m_ShouldClose;

    // Core Systems
    std::unique_ptr<IGame> m_Game;

    // Screens
    std::unique_ptr<GameplayScreen> m_GameplayScreen;
    std::unique_ptr<MainMenuScreen> m_MainMenuScreen;
    std::unique_ptr<PauseMenuScreen> m_PauseMenuScreen;
    std::unique_ptr<SettingsScreen> m_SettingsScreen;
    std::unique_ptr<LevelCompletedScreen> m_LevelCompletedScreen;
    std::unique_ptr<GameOverScreen> m_GameOverScreen;

    // Rendering
    RenderTexture2D m_RenderTarget;
    Shader m_CrtShader;
    int m_ResolutionLoc;
    int m_TimeLoc;
    float m_ElapsedTime;
    Font m_Font;
};
