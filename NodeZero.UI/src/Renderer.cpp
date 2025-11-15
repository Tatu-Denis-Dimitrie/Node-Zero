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

void Renderer::DrawSquareNode(float x, float y, float size, float hpPercentage, Color color) {
    if (hpPercentage > 0.0f) {
        float fillHeight = size * 2.0f * hpPercentage;
        DrawRectangle(
            static_cast<int>(x - size),
            static_cast<int>(y + size - fillHeight),
            static_cast<int>(size * 2),
            static_cast<int>(fillHeight),
            color);
    }

    DrawRectangleLines(
        static_cast<int>(x - size),
        static_cast<int>(y - size),
        static_cast<int>(size * 2),
        static_cast<int>(size * 2),
        BLACK);
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
