#pragma once

#include <functional>

#include "../../include/Widgets/Menu.h"
#include "Enums/GameState.h"
#include "IAppState.h"

class MainMenuState : public IAppState {
   public:
    MainMenuState(std::function<void(GameState)> stateChangeCallback);

    void Update(float deltaTime) override;
    void Draw() override;

   private:
    std::unique_ptr<Menu> m_Menu;
    std::function<void(GameState)> m_StateChangeCallback;
};
