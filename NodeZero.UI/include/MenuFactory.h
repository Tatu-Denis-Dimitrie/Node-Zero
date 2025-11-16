#pragma once

#include <memory>
#include "Enums/GameState.h"
#include "Widgets/Menu.h"

class MenuFactory
{
public:
    static std::unique_ptr<Menu> CreateMainMenu(GameState& currentState);
    static std::unique_ptr<Menu> CreatePauseMenu(GameState& currentState);
};
