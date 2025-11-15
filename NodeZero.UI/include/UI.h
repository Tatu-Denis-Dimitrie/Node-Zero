#pragma once

#include "raylib.h"

class UI
{
public:
    static void DrawDebugInfo(int posX, int posY);

    static void DrawTitle(const char* title, int posX, int posY, int fontSize, Color color);

    static void DrawScore(int score, int posX, int posY, int fontSize, Color color);

    static void DrawMultiplier(int multiplier, int posX, int posY, int fontSize, Color color);
};
