#pragma once

#include <functional>

#include "Enums/GameState.h"
#include "IAppState.h"
#include "IGame.h"
#include "raylib.h"

class SettingsState : public IAppState {
   public:
    SettingsState(IGame& game, std::function<void(GameState)> stateChangeCallback);

    void Update(float deltaTime) override;
    void Draw() override;

   private:
    IGame& m_Game;
    std::function<void(GameState)> m_StateChangeCallback;
};
