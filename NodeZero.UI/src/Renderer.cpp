#include "../include/Renderer.h"

#include <cmath>
#include <string>

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

// UI rendering methods (merged from UI.cpp)

void Renderer::DrawDebugInfo(int posX, int posY) {
    DrawFPS(posX, posY);
}

void Renderer::DrawTitle(const char* title, int posX, int posY, int fontSize, Color color) {
    DrawText(title, posX, posY, fontSize, color);
}

void Renderer::DrawScore(int score, int posX, int posY, int fontSize, Color color) {
    std::string scoreText = "Score: " + std::to_string(score);
    DrawText(scoreText.c_str(), posX, posY, fontSize, color);
}

void Renderer::DrawMultiplier(int multiplier, int posX, int posY, int fontSize, Color color) {
    if (multiplier > 1) {
        std::string multiplierText = "x" + std::to_string(multiplier);
        DrawText(multiplierText.c_str(), posX, posY, fontSize, color);
    }
}

void Renderer::DrawHealthBar(float health, float maxHealth, int posX, int posY, int width, int height) {
    DrawRectangle(posX, posY, width, height, Color{60, 60, 60, 255});

    float healthPercentage = health / maxHealth;
    if (healthPercentage < 0.0f) healthPercentage = 0.0f;
    if (healthPercentage > 1.0f) healthPercentage = 1.0f;

    int barWidth = static_cast<int>(width * healthPercentage);

    Color barColor;
    if (healthPercentage > 0.6f) {
        barColor = Color{0, 255, 100, 255};  // Green
    } else if (healthPercentage > 0.3f) {
        barColor = Color{255, 200, 0, 255};  // Yellow
    } else {
        barColor = Color{255, 50, 50, 255};  // Red
    }

    DrawRectangle(posX, posY, barWidth, height, barColor);

    DrawRectangleLines(posX, posY, width, height, WHITE);

    char healthBuffer[32];
    snprintf(healthBuffer, sizeof(healthBuffer), "%.1f / %.0f", health, maxHealth);
    DrawText(healthBuffer, posX + 5, posY + 2, 16, WHITE);
}