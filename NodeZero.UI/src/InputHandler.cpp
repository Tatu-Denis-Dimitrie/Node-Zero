#include "../include/InputHandler.h"
#include <cmath>

Vector2 InputHandler::GetMousePosition()
{
    return ::GetMousePosition();
}

bool InputHandler::IsMouseButtonPressed(int button)
{
    return ::IsMouseButtonPressed(button);
}

bool InputHandler::IsKeyPressed(int key)
{
    return ::IsKeyPressed(key);
}

bool InputHandler::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}

float InputHandler::CalculateAngle(float x1, float y1, float x2, float y2)
{
    float deltaX = x2 - x1;
    float deltaY = y2 - y1;
    float angleRadians = atan2f(deltaY, deltaX);
    float angleDegrees = angleRadians * RAD2DEG;

    // Ajustare pentru orientarea săgeții
    return angleDegrees + 90.0f;
}
