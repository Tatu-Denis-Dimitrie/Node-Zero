#include "Renderer.h"

#include <cmath>
#include <string>
#include <vector>

#include "raymath.h"

void Renderer::DrawCircleNode(float x, float y, float size, float hpPercentage, Color color, float rotation) {
    const int sides = 32;

    std::vector<Vector2> vertices;
    vertices.reserve(sides);
    for (int i = 0; i < sides; ++i) {
        float angle = (rotation + i * (360.0f / sides)) * DEG2RAD;
        vertices.push_back({x + size * cosf(angle), y + size * sinf(angle)});
    }

    if (hpPercentage > 0.0f) {
        if (hpPercentage >= 1.0f) {
            DrawPoly(Vector2{x, y}, sides, size, rotation, color);
        } else {
            float rad = rotation * DEG2RAD;
            Vector2 fillDir = {sinf(rad), -cosf(rad)};

            float limit = -size + hpPercentage * 2.0f * size;

            std::vector<Vector2> clippedVertices;
            Vector2 center = {x, y};

            Vector2 p1 = vertices.back();
            float dist1 = Vector2DotProduct(Vector2Subtract(p1, center), fillDir);
            bool p1Inside = (dist1 <= limit);

            for (const auto& p2 : vertices) {
                float dist2 = Vector2DotProduct(Vector2Subtract(p2, center), fillDir);
                bool p2Inside = (dist2 <= limit);

                if (p1Inside && p2Inside) {
                    clippedVertices.push_back(p2);
                } else if (p1Inside && !p2Inside) {
                    float t = (limit - dist1) / (dist2 - dist1);
                    Vector2 intersection = Vector2Add(p1, Vector2Scale(Vector2Subtract(p2, p1), t));
                    clippedVertices.push_back(intersection);
                } else if (!p1Inside && p2Inside) {
                    float t = (limit - dist1) / (dist2 - dist1);
                    Vector2 intersection = Vector2Add(p1, Vector2Scale(Vector2Subtract(p2, p1), t));
                    clippedVertices.push_back(intersection);
                    clippedVertices.push_back(p2);
                }

                p1 = p2;
                dist1 = dist2;
                p1Inside = p2Inside;
            }

            if (clippedVertices.size() >= 3) {
                Vector2 centerFan = clippedVertices[0];
                for (size_t i = 1; i < clippedVertices.size() - 1; ++i) {
                    DrawTriangle(centerFan, clippedVertices[i + 1], clippedVertices[i], color);
                }
            }
        }
    }

    float borderThickness = GetScreenHeight() * 0.003f;
    DrawPolyLinesEx(Vector2{x, y}, sides, size, rotation, borderThickness, color);
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

    float borderThickness = GetScreenHeight() * 0.003f;
    DrawLineEx(topLeft, topRight, borderThickness, color);
    DrawLineEx(topRight, bottomRight, borderThickness, color);
    DrawLineEx(bottomRight, bottomLeft, borderThickness, color);
    DrawLineEx(bottomLeft, topLeft, borderThickness, color);
}

void Renderer::DrawHexagonNode(float x, float y, float size, float hpPercentage, Color color, float rotation) {
    const int sides = 6;
    std::vector<Vector2> vertices;
    vertices.reserve(sides);
    for (int i = 0; i < sides; ++i) {
        float angle = DEG2RAD * (rotation + i * 60.0f);
        vertices.push_back({x + size * cosf(angle), y + size * sinf(angle)});
    }

    if (hpPercentage > 0.0f) {
        if (hpPercentage >= 1.0f) {
            DrawPoly(Vector2{x, y}, sides, size, rotation, color);
        } else {
            float rad = rotation * DEG2RAD;
            Vector2 fillDir = {sinf(rad), -cosf(rad)};
            float limit = -size + hpPercentage * 2.0f * size;

            std::vector<Vector2> clippedVertices;
            Vector2 center = {x, y};

            Vector2 p1 = vertices.back();
            float dist1 = Vector2DotProduct(Vector2Subtract(p1, center), fillDir);
            bool p1Inside = (dist1 <= limit);

            for (const auto& p2 : vertices) {
                float dist2 = Vector2DotProduct(Vector2Subtract(p2, center), fillDir);
                bool p2Inside = (dist2 <= limit);

                if (p1Inside && p2Inside) {
                    clippedVertices.push_back(p2);
                } else if (p1Inside && !p2Inside) {
                    float t = (limit - dist1) / (dist2 - dist1);
                    Vector2 intersection = Vector2Add(p1, Vector2Scale(Vector2Subtract(p2, p1), t));
                    clippedVertices.push_back(intersection);
                } else if (!p1Inside && p2Inside) {
                    float t = (limit - dist1) / (dist2 - dist1);
                    Vector2 intersection = Vector2Add(p1, Vector2Scale(Vector2Subtract(p2, p1), t));
                    clippedVertices.push_back(intersection);
                    clippedVertices.push_back(p2);
                }

                p1 = p2;
                dist1 = dist2;
                p1Inside = p2Inside;
            }

            if (clippedVertices.size() >= 3) {
                Vector2 centerFan = clippedVertices[0];
                for (size_t i = 1; i < clippedVertices.size() - 1; ++i) {
                    DrawTriangle(centerFan, clippedVertices[i + 1], clippedVertices[i], color);
                }
            }
        }
    }

    float borderThickness = GetScreenHeight() * 0.003f;
    DrawPolyLinesEx(Vector2{x, y}, sides, size, rotation, borderThickness, color);
}

void Renderer::DrawPickup(float x, float y, float size, Color color) {
    float thickness = GetScreenHeight() * 0.002f;
    DrawLineEx(Vector2{x - size, y}, Vector2{x + size, y}, thickness, color);
    DrawLineEx(Vector2{x, y - size}, Vector2{x, y + size}, thickness, color);
}

void Renderer::DrawDebugInfo(int posX, int posY, Font font) {
    std::string fpsText = "FPS: " + std::to_string(GetFPS());
    int fontSize = static_cast<int>(GetScreenHeight() * 0.025f);
    Vector2 textSize = MeasureTextEx(font, fpsText.c_str(), static_cast<float>(fontSize), 1);
    float alignedX = posX - textSize.x;
    DrawTextEx(font, fpsText.c_str(), Vector2{alignedX, static_cast<float>(posY)}, static_cast<float>(fontSize), 1, WHITE);
}

void Renderer::DrawPoints(int points, int posX, int posY, int fontSize, Color color, Font font) {
    std::string pointsText = "Points: " + std::to_string(points);
    DrawTextEx(font, pointsText.c_str(), Vector2{static_cast<float>(posX), static_cast<float>(posY)}, static_cast<float>(fontSize), 1, color);
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

    Color barColor = Color{255, 50, 50, 255};

    DrawRectangle(posX, posY, barWidth, height, barColor);

    DrawRectangleLines(posX, posY, width, height, WHITE);

    char healthBuffer[32];
    snprintf(healthBuffer, sizeof(healthBuffer), "%.1f / %.0f", health, maxHealth);
    int healthTextSize = static_cast<int>(height * 0.65f);
    Vector2 textSize = MeasureTextEx(font, healthBuffer, static_cast<float>(healthTextSize), 1);
    float textX = posX + (width - textSize.x) / 2.0f;
    float textY = posY + (height - textSize.y) / 2.0f;
    DrawTextEx(font, healthBuffer, Vector2{textX, textY}, static_cast<float>(healthTextSize), 1, WHITE);
}

void Renderer::DrawProgressBar(float percentage, int currentLevel, Font font) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int progressBarHeight = static_cast<int>(screenHeight * 0.03f);
    int progressBarMargin = static_cast<int>(screenHeight * 0.02f);
    int progressBarSideMargin = static_cast<int>(screenWidth * 0.015f);
    int progressBarY = screenHeight - progressBarHeight - progressBarMargin;
    int progressBarX = progressBarSideMargin;
    int progressBarWidth = screenWidth - (progressBarSideMargin * 2);

    DrawRectangle(progressBarX, progressBarY, progressBarWidth, progressBarHeight, Color{20, 20, 30, 220});

    int fillWidth = static_cast<int>(progressBarWidth * (percentage / 100.0f));
    DrawRectangle(progressBarX, progressBarY, fillWidth, progressBarHeight, Color{255, 255, 255, 255});

    DrawRectangleLines(progressBarX, progressBarY, progressBarWidth, progressBarHeight, Color{255, 255, 255, 255});

    char progressLabel[64];
    snprintf(progressLabel, sizeof(progressLabel), "Level %d - progress", currentLevel);
    int labelFontSize = static_cast<int>(screenHeight * 0.025f);
    Vector2 labelSize = MeasureTextEx(font, progressLabel, static_cast<float>(labelFontSize), 1);
    DrawTextEx(font, progressLabel, Vector2{static_cast<float>((screenWidth - labelSize.x) / 2), static_cast<float>(progressBarY - screenHeight * 0.035f)}, static_cast<float>(labelFontSize), 1, Color{200, 200, 200, 255});
}