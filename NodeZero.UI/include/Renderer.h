#pragma once

#include "raylib.h"

class Renderer {
   public:
    static void DrawCircleNode(float x, float y, float size, float hpPercentage, Color color);

    static void DrawSquareNode(float x, float y, float size, float hpPercentage, Color color);

    static void DrawTriangleNode(float x, float y, float size, float hpPercentage, Color color);
};
