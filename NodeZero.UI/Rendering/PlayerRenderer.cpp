#include "PlayerRenderer.h"
#include "raymath.h"
#include <cmath>

void PlayerRenderer::DrawArrow(float x, float y, float size, float rotationDegrees, Color color)
{
    float rotation = rotationDegrees * DEG2RAD;
    
    Vector2 point1 = { 0.0f, -size };    
    Vector2 point2 = { -size * 0.6f, size };    
    Vector2 point3 = { size * 0.6f, size };  
    
    float cosRot = cosf(rotation);
    float sinRot = sinf(rotation);
    
    Vector2 rotatedPoint1 = {
        point1.x * cosRot - point1.y * sinRot + x,
        point1.x * sinRot + point1.y * cosRot + y
    };
    
    Vector2 rotatedPoint2 = {
        point2.x * cosRot - point2.y * sinRot + x,
        point2.x * sinRot + point2.y * cosRot + y
    };
    
    Vector2 rotatedPoint3 = {
        point3.x * cosRot - point3.y * sinRot + x,
        point3.x * sinRot + point3.y * cosRot + y
    };
    
    DrawTriangle(rotatedPoint1, rotatedPoint2, rotatedPoint3, color);
    DrawTriangleLines(rotatedPoint1, rotatedPoint2, rotatedPoint3, BLACK);
}
