#include "../include/GameFactory.h"
#include "../include/IGame.h"
#include "Systems/CollisionSystem.h"
#include "Systems/ScoreSystem.h"
#include "../include/Config/GameConfig.h"

#include "Game.h"

// Game management
IGame* GameFactory::CreateGame()
{
    return new Game();
}

void GameFactory::DestroyGame(IGame* game)
{
    delete game;
}

// CollisionSystem management
ICollisionSystem* GameFactory::CreateCollisionSystem()
{
    return new CollisionSystem();
}

void GameFactory::DestroyCollisionSystem(ICollisionSystem* system)
{
    delete system;
}

// ScoreSystem management
IScoreSystem* GameFactory::CreateScoreSystem()
{
    return new ScoreSystem();
}

void GameFactory::DestroyScoreSystem(IScoreSystem* system)
{
    delete system;
}
