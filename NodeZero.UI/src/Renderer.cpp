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

    DrawCircleLines(static_cast<int>(x), static_cast<int>(y), size, RED);
}

void Renderer::DrawSquareNode(float x, float y, float size, float hpPercentage, Color color, float rotation) {
    float rad = rotation * DEG2RAD;
    float cosA = cosf(rad);
    float sinA = sinf(rad);

    Vector2 topLeft = {x + (-size * cosA - (-size) * sinA), y + (-size * sinA + (-size) * cosA)};
    Vector2 topRight = {x + (size * cosA - (-size) * sinA), y + (size * sinA + (-size) * cosA)};
    Vector2 bottomLeft = {x + (-size * cosA - size * sinA), y + (-size * sinA + size * cosA)};
    Vector2 bottomRight = {x + (size * cosA - size * sinA), y + (size * sinA + size * cosA)};

    if (hpPercentage > 0.0f) {
        float fillRatio = hpPercentage;

        Vector2 fillTopLeft = {
            bottomLeft.x + (topLeft.x - bottomLeft.x) * fillRatio,
            bottomLeft.y + (topLeft.y - bottomLeft.y) * fillRatio};
        Vector2 fillTopRight = {
            bottomRight.x + (topRight.x - bottomRight.x) * fillRatio,
            bottomRight.y + (topRight.y - bottomRight.y) * fillRatio};

        DrawTriangle(bottomLeft, fillTopRight, fillTopLeft, color);
        DrawTriangle(bottomLeft, bottomRight, fillTopRight, color);
    }

    float borderThickness = 3.0f;
    DrawLineEx(topLeft, topRight, borderThickness, RED);
    DrawLineEx(topRight, bottomRight, borderThickness, RED);
    DrawLineEx(bottomRight, bottomLeft, borderThickness, RED);
    DrawLineEx(bottomLeft, topLeft, borderThickness, RED);
}

void Renderer::DrawHexagonNode(float x, float y, float size, float hpPercentage, Color color, float rotation) {
    const int sides = 6;
    Vector2 vertices[sides];
    for (int i = 0; i < sides; ++i) {
        float angle = DEG2RAD * (rotation + i * 60.0f);
        vertices[i] = {x + size * cosf(angle), y + size * sinf(angle)};
    }

    if (hpPercentage > 0.0f) {
        float minY = y - size;
        float maxY = y + size;
        float height = maxY - minY;

        float fillHeight = height * hpPercentage;
        float currentFillY = maxY - fillHeight;

        BeginScissorMode(
            static_cast<int>(x - size),
            static_cast<int>(currentFillY),
            static_cast<int>(size * 2),
            static_cast<int>(fillHeight));

        DrawPoly(Vector2{x, y}, sides, size, rotation, color);

        EndScissorMode();
    }

    DrawPolyLinesEx(Vector2{x, y}, sides, size, rotation, 3.0f, RED);
}

void Renderer::DrawPickup(float x, float y, float size, Color color) {
    float thickness = 2.0f;
    DrawLineEx(Vector2{x - size, y}, Vector2{x + size, y}, thickness, color);
    DrawLineEx(Vector2{x, y - size}, Vector2{x, y + size}, thickness, color);
}