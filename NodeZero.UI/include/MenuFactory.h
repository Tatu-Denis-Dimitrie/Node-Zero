#pragma once

#include <memory>
#include "Enums/GameState.h"
#include "Widgets/Menu.h"

class IGame;

class MenuFactory
{
public:
    static std::unique_ptr<Menu> CreateMainMenu(GameState& currentState);
    static std::unique_ptr<Menu> CreatePauseMenu(GameState& currentState, IGame& game, float& spawnTimer, float& currentHealth, float maxHealth, float& healthTimer);
};
