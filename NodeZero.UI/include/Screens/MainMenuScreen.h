#pragma once

#include <functional>

#include "../../include/Widgets/Menu.h"
#include "Enums/GameScreen.h"
#include "IAppScreen.h"

class MainMenuScreen : public IAppScreen {
   public:
    MainMenuScreen(std::function<void(GameScreen)> stateChangeCallback);

    void Update(float deltaTime) override;
    void Draw() override;

   private:
    std::unique_ptr<Menu> m_Menu;
    std::function<void(GameScreen)> m_StateChangeCallback;
};
