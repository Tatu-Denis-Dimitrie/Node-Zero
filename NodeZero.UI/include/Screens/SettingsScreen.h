#pragma once

#include <functional>

#include "Enums/GameScreen.h"
#include "IGame.h"
#include "raylib.h"

class SettingsScreen {
   public:
    SettingsScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback);

    void Update(float deltaTime);
    void Draw();

   private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    bool m_WasMousePressed;
    bool m_IsFirstFrame;
};
