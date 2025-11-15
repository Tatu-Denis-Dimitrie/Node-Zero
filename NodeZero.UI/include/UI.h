#pragma once

#include "raylib.h"

/// <summary>
/// Gestionează elementele UI (HUD)
/// </summary>
class UI
{
public:
    /// <summary>
    /// Desenează informații de debug (FPS, etc.)
    /// </summary>
    static void DrawDebugInfo(int posX, int posY);

    /// <summary>
    /// Desenează un titlu
    /// </summary>
    static void DrawTitle(const char* title, int posX, int posY, int fontSize, Color color);

    /// <summary>
    /// Desenează scorul
    /// </summary>
    static void DrawScore(int score, int posX, int posY, int fontSize, Color color);

    /// <summary>
    /// Desenează multiplicatorul
    /// </summary>
    static void DrawMultiplier(int multiplier, int posX, int posY, int fontSize, Color color);
};
