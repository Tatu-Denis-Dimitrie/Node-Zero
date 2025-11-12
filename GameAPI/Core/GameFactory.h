#pragma once

#include "IGame.h"

// Factory pentru crearea instantelor de joc
// Frontend-ul foloseste doar aceasta functie, nu clasele concrete
class GameFactory
{
public:
    // Creaza o noua instanta de joc
    static IGame* CreateGame();
    
    // Distruge instanta de joc
    static void DestroyGame(IGame* game);
};
