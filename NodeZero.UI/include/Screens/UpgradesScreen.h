#pragma once

#include <functional>

#include "Enums/GameScreen.h"
#include "IGame.h"
#include "raylib.h"

class UpgradesScreen {
   public:
    UpgradesScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

   private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    bool m_WasMousePressed;
    bool m_IsFirstFrame;
    Font m_Font;
};
