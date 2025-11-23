#pragma once

#include <functional>

#include "../../include/Widgets/Menu.h"
#include "Enums/GameScreen.h"
#include "IGame.h"

class PauseMenuScreen {
   public:
    PauseMenuScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback);

    void Update(float deltaTime);
    void Draw();

   private:
    std::unique_ptr<Menu> m_Menu;
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
};
