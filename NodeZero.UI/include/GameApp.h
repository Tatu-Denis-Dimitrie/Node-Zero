#pragma once

#include <memory>

#include "Enums/GameScreen.h"
#include "raylib.h"

class IGame;
class GameplayScreen;
class MainScreen;
class PauseScreen;
class UpgradesScreen;
class LevelCompletedScreen;
class GameoverScreen;

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
    std::shared_ptr<GameplayScreen> m_GameplayScreen;
    std::unique_ptr<MainScreen> m_MainScreen;
    std::unique_ptr<PauseScreen> m_PauseScreen;
    std::unique_ptr<UpgradesScreen> m_UpgradesScreen;
    std::unique_ptr<LevelCompletedScreen> m_LevelCompletedScreen;
    std::unique_ptr<GameoverScreen> m_GameoverScreen;

    // Rendering
    RenderTexture2D m_RenderTarget;
    Shader m_CrtShader;
    int m_ResolutionLoc;
    int m_TimeLoc;
    float m_ElapsedTime;
    Font m_Font;
};
