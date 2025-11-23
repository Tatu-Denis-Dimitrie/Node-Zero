#pragma once

#include "raylib.h"

class Renderer
{
public:
    // Game entity rendering
    static void DrawCircleNode(float x, float y, float size, float hpPercentage, Color color);
    static void DrawSquareNode(float x, float y, float size, float hpPercentage, Color color, float rotation = 0.0f);
    static void DrawHexagonNode(float x, float y, float size, float hpPercentage, Color color, float rotation = 0.0f);
    static void DrawPickup(float x, float y, float size, Color color);

    // UI rendering (merged from UI.h)
    static void DrawDebugInfo(int posX, int posY);
    static void DrawTitle(const char* title, int posX, int posY, int fontSize, Color color);
    static void DrawScore(int score, int posX, int posY, int fontSize, Color color);
    static void DrawMultiplier(int multiplier, int posX, int posY, int fontSize, Color color);
    static void DrawHealthBar(float health, float maxHealth, int posX, int posY, int width, int height);
};
