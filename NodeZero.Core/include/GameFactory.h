#pragma once

#include <memory>

class IGame;
class ICollisionSystem;
class IScoreSystem;

class GameFactory {
   public:
    static std::unique_ptr<IGame> CreateGame();
    static std::unique_ptr<ICollisionSystem> CreateCollisionSystem();
    static std::unique_ptr<IScoreSystem> CreateScoreSystem();
};
