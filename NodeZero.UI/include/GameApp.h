#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Enums/GameState.h"
#include "States/IAppState.h"
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

    void ChangeState(GameState newState);

    // Game State
    GameState m_CurrentState;
    GameState m_PreviousState;
    bool m_ShouldClose;

    // Core Systems
    std::unique_ptr<IGame> m_Game;

    // States
    std::map<GameState, std::unique_ptr<IAppState>> m_States;

    // Rendering
    RenderTexture2D m_RenderTarget;
    Shader m_CrtShader;
    int m_ResolutionLoc;
    int m_TimeLoc;
    float m_ElapsedTime;
};
