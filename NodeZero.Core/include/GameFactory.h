#pragma once

class IGame;

class GameFactory
{
public:
    static IGame* CreateGame();

    static void DestroyGame(IGame* game);
};
