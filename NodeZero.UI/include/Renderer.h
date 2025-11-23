#pragma once

#include "raylib.h"

class Renderer {
   public:
    static void DrawCircleNode(float x, float y, float size, float hpPercentage, Color color, float rotation = 0.0f);
    static void DrawSquareNode(float x, float y, float size, float hpPercentage, Color color, float rotation = 0.0f);
    static void DrawHexagonNode(float x, float y, float size, float hpPercentage, Color color, float rotation = 0.0f);
    static void DrawPickup(float x, float y, float size, Color color);

    static void DrawDebugInfo(int posX, int posY, Font font);
    static void DrawScore(int score, int posX, int posY, int fontSize, Color color, Font font);
    static void DrawMultiplier(int multiplier, int posX, int posY, int fontSize, Color color, Font font);
    static void DrawHealthBar(float health, float maxHealth, int posX, int posY, int width, int height, Font font);
    static void DrawProgressBar(float percentage, int currentLevel, Font font);
};
