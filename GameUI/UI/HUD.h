#pragma once

#include "raylib.h"

// Modul pentru interfata utilizator (HUD)
class HUD
{
public:
    // Deseneaza informatii de debug si UI
    static void DrawDebugInfo(int posX, int posY);
    
    // Deseneaza titlul jocului
    static void DrawTitle(const char* title, int posX, int posY, int fontSize, Color color);
};
