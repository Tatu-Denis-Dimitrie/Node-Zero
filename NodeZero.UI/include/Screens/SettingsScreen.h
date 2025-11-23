#pragma once

#include <functional>

#include "Enums/GameScreen.h"
#include "IAppScreen.h"
#include "IGame.h"
#include "raylib.h"

class SettingsScreen : public IAppScreen {
   public:
    SettingsScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback);

    void Update(float deltaTime) override;
    void Draw() override;

   private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    bool m_WasMousePressed;
    bool m_IsFirstFrame;
};
