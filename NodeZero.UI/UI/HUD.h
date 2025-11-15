#pragma once

#include "raylib.h"

class HUD {
   public:
    static void DrawDebugInfo(int posX, int posY);

    static void DrawTitle(const char* title, int posX, int posY, int fontSize, Color color);
};
