#pragma once

#include <functional>

#include "../../include/Widgets/Menu.h"
#include "Enums/GameScreen.h"

class MainMenuScreen {
   public:
    MainMenuScreen(std::function<void(GameScreen)> stateChangeCallback);

    void Update(float deltaTime);
    void Draw();

   private:
    std::unique_ptr<Menu> m_Menu;
    std::function<void(GameScreen)> m_StateChangeCallback;
};
