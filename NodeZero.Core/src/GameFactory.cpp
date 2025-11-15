#include "../include/GameFactory.h"

#include "../include/Config/GameConfig.h"
#include "../include/IGame.h"
#include "Game.h"
#include "Systems/CollisionSystem.h"
#include "Systems/ScoreSystem.h"

std::unique_ptr<IGame> GameFactory::CreateGame() {
    return std::make_unique<Game>();
}

std::unique_ptr<ICollisionSystem> GameFactory::CreateCollisionSystem() {
    return std::make_unique<CollisionSystem>();
}

std::unique_ptr<IScoreSystem> GameFactory::CreateScoreSystem() {
    return std::make_unique<ScoreSystem>();
}
