#pragma once

// Forward declarations
class IGame;
class ICollisionSystem;
class IScoreSystem;

/// <summary>
/// Factory pentru crearea instanțelor de joc și sisteme
/// </summary>
class GameFactory
{
public:
    // Game management
    static IGame* CreateGame();
    static void DestroyGame(IGame* game);

    // Systems management
    static ICollisionSystem* CreateCollisionSystem();
    static void DestroyCollisionSystem(ICollisionSystem* system);

    static IScoreSystem* CreateScoreSystem();
    static void DestroyScoreSystem(IScoreSystem* system);
};
