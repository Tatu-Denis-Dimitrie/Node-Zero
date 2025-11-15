#include "../include/Renderer.h"
#include "raymath.h"
#include <cmath>

void Renderer::DrawArrow(float x, float y, float size, float rotationDegrees, Color color)
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

void Renderer::DrawCircleNode(float x, float y, float size, float hpPercentage, Color color)
{
    // Desenează conturul complet (gol)
    DrawCircleLines(static_cast<int>(x), static_cast<int>(y), size, BLACK);

    // Desenează umplerea bazată pe HP
    if (hpPercentage > 0.0f)
    {
        float fillRadius = size * hpPercentage;
        DrawCircleV({ x, y }, fillRadius, color);
    }
}

void Renderer::DrawSquareNode(float x, float y, float size, float hpPercentage, Color color)
{
    // Desenează conturul complet (gol)
    DrawRectangleLines(
        static_cast<int>(x - size),
        static_cast<int>(y - size),
        static_cast<int>(size * 2),
        static_cast<int>(size * 2),
        BLACK
    );

    // Desenează umplerea bazată pe HP
    if (hpPercentage > 0.0f)
    {
        float fillSize = size * hpPercentage;
        DrawRectangle(
            static_cast<int>(x - fillSize),
            static_cast<int>(y - fillSize),
            static_cast<int>(fillSize * 2),
            static_cast<int>(fillSize * 2),
            color
        );
    }
}

void Renderer::DrawTriangleNode(float x, float y, float size, float hpPercentage, Color color)
{
    // Punctele pentru conturul complet
    Vector2 point1 = { x, y - size };
    Vector2 point2 = { x - size * 0.866f, y + size * 0.5f };
    Vector2 point3 = { x + size * 0.866f, y + size * 0.5f };

    // Desenează conturul complet (gol)
    DrawTriangleLines(point1, point2, point3, BLACK);

    // Desenează umplerea bazată pe HP
    if (hpPercentage > 0.0f)
    {
        float fillSize = size * hpPercentage;
        Vector2 fillPoint1 = { x, y - fillSize };
        Vector2 fillPoint2 = { x - fillSize * 0.866f, y + fillSize * 0.5f };
        Vector2 fillPoint3 = { x + fillSize * 0.866f, y + fillSize * 0.5f };

        DrawTriangle(fillPoint1, fillPoint2, fillPoint3, color);
    }
}
