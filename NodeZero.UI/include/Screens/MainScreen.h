#pragma once

#include <functional>

#include "Enums/GameScreen.h"
#include "Widgets/Menu.h"
#include "raylib.h"

class MainScreen {
   public:
    MainScreen(std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

   private:
    std::unique_ptr<Menu> m_Menu;
    std::function<void(GameScreen)> m_StateChangeCallback;
    Font m_Font;
};
