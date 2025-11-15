#pragma once

#include "raylib.h"

/// <summary>
/// Gestionează input-ul de la utilizator
/// </summary>
class InputHandler
{
public:
    /// <summary>
    /// Obține poziția mouse-ului
    /// </summary>
    static Vector2 GetMousePosition();

    /// <summary>
    /// Verifică dacă un buton de mouse este apăsat
    /// </summary>
    static bool IsMouseButtonPressed(int button);

    /// <summary>
    /// Verifică dacă o tastă este apăsată
    /// </summary>
    static bool IsKeyPressed(int key);

    /// <summary>
    /// Verifică dacă o tastă este ținută apăsată
    /// </summary>
    static bool IsKeyDown(int key);

    /// <summary>
    /// Calculează unghiul între două puncte
    /// </summary>
    static float CalculateAngle(float x1, float y1, float x2, float y2);
};
