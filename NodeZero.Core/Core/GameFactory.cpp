#include "GameFactory.h"
#include "../Entities/Game.h"

IGame* GameFactory::CreateGame()
{
    return new Game();
}

void GameFactory::DestroyGame(IGame* game)
{
    delete game;
}
