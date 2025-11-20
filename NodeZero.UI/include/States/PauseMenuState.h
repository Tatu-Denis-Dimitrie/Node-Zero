#pragma once

#include <functional>

#include "../../include/Widgets/Menu.h"
#include "Enums/GameState.h"
#include "IAppState.h"
#include "IGame.h"

class PauseMenuState : public IAppState {
   public:
    PauseMenuState(IGame& game, std::function<void(GameState)> stateChangeCallback);

    void Update(float deltaTime) override;
    void Draw() override;

   private:
    std::unique_ptr<Menu> m_Menu;
    IGame& m_Game;
    std::function<void(GameState)> m_StateChangeCallback;
};
