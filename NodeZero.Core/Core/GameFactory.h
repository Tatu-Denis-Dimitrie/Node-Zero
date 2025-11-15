#pragma once

#include "IGame.h"

class GameFactory
{
public:
    static IGame* CreateGame();
    
    static void DestroyGame(IGame* game);
};
