#pragma once

#include "raylib.h"

/// <summary>
/// Sistem principal de rendering pentru joc
/// </summary>
class Renderer
{
public:
    /// <summary>
    /// Desenează o săgeată (player)
    /// </summary>
    static void DrawArrow(float x, float y, float size, float rotationDegrees, Color color);

    /// <summary>
    /// Desenează un nod circular cu HP
    /// </summary>
    static void DrawCircleNode(float x, float y, float size, float hpPercentage, Color color);

    /// <summary>
    /// Desenează un nod pătrat cu HP
    /// </summary>
    static void DrawSquareNode(float x, float y, float size, float hpPercentage, Color color);

    /// <summary>
    /// Desenează un nod triunghiular cu HP
    /// </summary>
    static void DrawTriangleNode(float x, float y, float size, float hpPercentage, Color color);
};
