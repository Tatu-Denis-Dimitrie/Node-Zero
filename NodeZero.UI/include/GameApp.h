#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Enums/GameScreen.h"
#include "Screens/IAppScreen.h"
#include "raylib.h"

class IGame;

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
    std::map<GameScreen, std::unique_ptr<IAppScreen>> m_States;

    // Rendering
    RenderTexture2D m_RenderTarget;
    Shader m_CrtShader;
    int m_ResolutionLoc;
    int m_TimeLoc;
    float m_ElapsedTime;
};
