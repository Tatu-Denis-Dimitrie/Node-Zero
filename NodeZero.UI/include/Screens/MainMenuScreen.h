#pragma once

#include <functional>

#include "../../include/Widgets/Menu.h"
#include "Enums/GameScreen.h"
#include "raylib.h"

class MainMenuScreen {
   public:
    MainMenuScreen(std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

   private:
    std::unique_ptr<Menu> m_Menu;
    std::function<void(GameScreen)> m_StateChangeCallback;
    Font m_Font;
};
