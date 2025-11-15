#include "../include/GameFactory.h"
#include "../include/IGame.h"
#include "../include/Config/GameConfig.h"

#include "Game.cpp"

IGame* GameFactory::CreateGame()
{
    return new Game();
}

void GameFactory::DestroyGame(IGame* game)
{
    delete game;
}
