#pragma once

#include "raylib.h"

// Modul de rendering pentru jucator (sageata)
class PlayerRenderer
{
public:
    // Deseneaza un triunghi rotit (sageata)
    static void DrawArrow(float x, float y, float size, float rotationDegrees, Color color);
};
