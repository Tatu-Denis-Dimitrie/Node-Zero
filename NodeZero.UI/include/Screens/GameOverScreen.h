#pragma once

#include <functional>
#include <memory>

#include "Enums/GameScreen.h"
#include "IGame.h"
#include "Widgets/Menu.h"
#include "raylib.h"

class GameOverScreen {
   public:
    GameOverScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);
    void Update(float deltaTime);
    void Draw();

   private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    std::unique_ptr<Menu> m_Menu;
    Font m_Font;
};
