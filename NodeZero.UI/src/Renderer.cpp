#include "../include/Renderer.h"

#include <cmath>

#include "raymath.h"

void Renderer::DrawCircleNode(float x, float y, float size, float hpPercentage, Color color) {
    if (hpPercentage > 0.0f) {
        float fillHeight = size * 2.0f * hpPercentage;
        float topY = y + size - fillHeight;

        for (int i = 0; i < static_cast<int>(fillHeight); i++) {
            float currentY = y + size - i;
            float distFromCenter = currentY - y;

            float widthAtHeight = sqrtf(size * size - distFromCenter * distFromCenter);

            if (widthAtHeight > 0) {
                DrawLine(
                    static_cast<int>(x - widthAtHeight),
                    static_cast<int>(currentY),
                    static_cast<int>(x + widthAtHeight),
                    static_cast<int>(currentY),
                    color);
            }
        }
    }

    DrawCircleLines(static_cast<int>(x), static_cast<int>(y), size, BLACK);
}

void Renderer::DrawSquareNode(float x, float y, float size, float hpPercentage, Color color, float rotation) {
    // Calcul rotație
    float rad = rotation * DEG2RAD;
    float cosA = cosf(rad);
    float sinA = sinf(rad);

    // Calculare colțuri pătratului rotat în jurul centrului (x, y)
    Vector2 topLeft = {x + (-size * cosA - (-size) * sinA), y + (-size * sinA + (-size) * cosA)};
    Vector2 topRight = {x + (size * cosA - (-size) * sinA), y + (size * sinA + (-size) * cosA)};
    Vector2 bottomLeft = {x + (-size * cosA - size * sinA), y + (-size * sinA + size * cosA)};
    Vector2 bottomRight = {x + (size * cosA - size * sinA), y + (size * sinA + size * cosA)};

    // Desenare fill HP (de jos în sus)
    if (hpPercentage > 0.0f) {
        // Calculăm punctele pentru fill-ul HP
        // Fill se face de la partea de jos (bottomLeft-bottomRight) până la un procent din înălțime
        float fillRatio = hpPercentage;

        // Puncte intermediare pentru partea de sus a fill-ului (interpolare de la bottom spre top)
        Vector2 fillTopLeft = {
            bottomLeft.x + (topLeft.x - bottomLeft.x) * fillRatio,
            bottomLeft.y + (topLeft.y - bottomLeft.y) * fillRatio
        };
        Vector2 fillTopRight = {
            bottomRight.x + (topRight.x - bottomRight.x) * fillRatio,
            bottomRight.y + (topRight.y - bottomRight.y) * fillRatio
        };

        // Desenare fill ca un quad (2 triunghiuri)
        DrawTriangle(bottomLeft, fillTopRight, fillTopLeft, color);
        DrawTriangle(bottomLeft, bottomRight, fillTopRight, color);
    }

    // Desenare contur rotat (gros și roșu)
    float borderThickness = 3.0f;
    DrawLineEx(topLeft, topRight, borderThickness, RED);
    DrawLineEx(topRight, bottomRight, borderThickness, RED);
    DrawLineEx(bottomRight, bottomLeft, borderThickness, RED);
    DrawLineEx(bottomLeft, topLeft, borderThickness, RED);
}

void Renderer::DrawTriangleNode(float x, float y, float size, float hpPercentage, Color color) {
    Vector2 point1 = {x, y - size};
    Vector2 point2 = {x - size * 0.866f, y + size * 0.5f};
    Vector2 point3 = {x + size * 0.866f, y + size * 0.5f};

    if (hpPercentage > 0.0f) {
        float triangleHeight = size * 1.5f;
        float fillHeight = triangleHeight * hpPercentage;

        for (int i = 0; i < static_cast<int>(fillHeight); i++) {
            float currentY = y + size * 0.5f - i;
            float distFromBottom = i;
            float distFromTop = triangleHeight - distFromBottom;

            float widthAtHeight = (size * 0.866f * 2.0f) * (distFromTop / triangleHeight);

            if (widthAtHeight > 0) {
                DrawLine(
                    static_cast<int>(x - widthAtHeight / 2.0f),
                    static_cast<int>(currentY),
                    static_cast<int>(x + widthAtHeight / 2.0f),
                    static_cast<int>(currentY),
                    color);
            }
        }
    }

    DrawTriangleLines(point1, point2, point3, BLACK);
}

void Renderer::DrawHexagonNode(float x, float y, float size, float hpPercentage, Color color, float rotation) {
    const int sides = 6;
    Vector2 vertices[sides];
    for (int i = 0; i < sides; ++i) {
        float angle = DEG2RAD * (rotation + i * 60.0f);
        vertices[i] = {x + size * cosf(angle), y + size * sinf(angle)};
    }

    if (hpPercentage > 0.0f) {
        float innerRadius = size * hpPercentage;
        DrawPoly(Vector2{x, y}, sides, innerRadius, rotation, color);
    }

    DrawPolyLinesEx(Vector2{x, y}, sides, size, rotation, 3.0f, DARKGREEN);
}