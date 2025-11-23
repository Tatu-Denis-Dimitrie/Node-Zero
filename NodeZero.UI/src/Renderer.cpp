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

void Renderer::DrawDebugInfo(int posX, int posY, Font font) {
    std::string fpsText = "FPS: " + std::to_string(GetFPS());
    DrawTextEx(font, fpsText.c_str(), Vector2{static_cast<float>(posX), static_cast<float>(posY)}, 20, 1, WHITE);
}

void Renderer::DrawTitle(const char* title, int posX, int posY, int fontSize, Color color, Font font) {
    DrawTextEx(font, title, Vector2{static_cast<float>(posX), static_cast<float>(posY)}, static_cast<float>(fontSize), 1, color);
}

void Renderer::DrawScore(int score, int posX, int posY, int fontSize, Color color, Font font) {
    std::string scoreText = "Score: " + std::to_string(score);
    DrawTextEx(font, scoreText.c_str(), Vector2{static_cast<float>(posX), static_cast<float>(posY)}, static_cast<float>(fontSize), 1, color);
}

void Renderer::DrawMultiplier(int multiplier, int posX, int posY, int fontSize, Color color, Font font) {
    if (multiplier > 1) {
        std::string multiplierText = "x" + std::to_string(multiplier);
        DrawTextEx(font, multiplierText.c_str(), Vector2{static_cast<float>(posX), static_cast<float>(posY)}, static_cast<float>(fontSize), 1, color);
    }
}

void Renderer::DrawHealthBar(float health, float maxHealth, int posX, int posY, int width, int height, Font font) {
    DrawRectangle(posX, posY, width, height, Color{60, 60, 60, 255});

    float healthPercentage = health / maxHealth;
    if (healthPercentage < 0.0f) healthPercentage = 0.0f;
    if (healthPercentage > 1.0f) healthPercentage = 1.0f;

    int barWidth = static_cast<int>(width * healthPercentage);

    // Always red
    Color barColor = Color{255, 50, 50, 255};

    DrawRectangle(posX, posY, barWidth, height, barColor);

    DrawRectangleLines(posX, posY, width, height, WHITE);

    char healthBuffer[32];
    snprintf(healthBuffer, sizeof(healthBuffer), "%.1f / %.0f", health, maxHealth);
    DrawTextEx(font, healthBuffer, Vector2{static_cast<float>(posX + 5), static_cast<float>(posY + 2)}, 16, 1, WHITE);
}

void Renderer::DrawProgressBar(float percentage, int currentLevel, Font font) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int progressBarHeight = 25;
    int progressBarMargin = 10;
    int progressBarSideMargin = 20;
    int progressBarY = screenHeight - progressBarHeight - progressBarMargin;
    int progressBarX = progressBarSideMargin;
    int progressBarWidth = screenWidth - (progressBarSideMargin * 2);

    // Background
    DrawRectangle(progressBarX, progressBarY, progressBarWidth, progressBarHeight, Color{20, 20, 30, 220});

    // Fill
    int fillWidth = static_cast<int>(progressBarWidth * (percentage / 100.0f));
    DrawRectangle(progressBarX, progressBarY, fillWidth, progressBarHeight, Color{255, 255, 255, 255});

    // Border
    DrawRectangleLines(progressBarX, progressBarY, progressBarWidth, progressBarHeight, Color{255, 255, 255, 255});

    // "progress" label above the bar
    char progressLabel[64];
    snprintf(progressLabel, sizeof(progressLabel), "Level %d - progress", currentLevel);
    Vector2 labelSize = MeasureTextEx(font, progressLabel, 20, 1);
    DrawTextEx(font, progressLabel, Vector2{static_cast<float>((screenWidth - labelSize.x) / 2), static_cast<float>(progressBarY - 30)}, 20, 1, Color{200, 200, 200, 255});
}