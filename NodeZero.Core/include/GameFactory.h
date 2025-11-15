#pragma once

// Forward declarations
class IGame;

/// <summary>
/// Factory pentru crearea instanțelor de joc și entități
/// </summary>
class GameFactory
{
public:
    /// <summary>
    /// Creează o instanță de joc
    /// </summary>
    static IGame* CreateGame();

    /// <summary>
    /// Distruge o instanță de joc
    /// </summary>
    static void DestroyGame(IGame* game);
};
