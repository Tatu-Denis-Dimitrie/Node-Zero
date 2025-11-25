#pragma once

#include <functional>

#include "Enums/GameScreen.h"
#include "IGame.h"
#include "Widgets/Menu.h"
#include "raylib.h"

class PauseScreen {
   public:
    PauseScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

   private:
    std::unique_ptr<Menu> m_Menu;
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
};
