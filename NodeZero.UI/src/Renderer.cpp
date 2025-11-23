#include "../include/Renderer.h"

#include <cmath>
#include <string>
#include <vector>

#include "raymath.h"

void Renderer::DrawCircleNode(float x, float y, float size, float hpPercentage, Color color, float rotation) {
    const int sides = 32;

    // Generate vertices for the circle (polygon)
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
            // Calculate fill direction (Up vector rotated)
            // When rotation is 0, Up is (0, -1) (Visual Up)
            // But we want to fill from Bottom to Top.
            // Bottom is +Y (in screen), Top is -Y.
            // So we fill from +Y towards -Y.
            // Let's define the "Up" vector as the direction of growth.
            // Growth is from Bottom to Top.
            // Local Up is (0, -1).
            // Rotated Up:
            float rad = rotation * DEG2RAD;
            Vector2 fillDir = {sinf(rad), -cosf(rad)};

            // We want to keep vertices that are "below" the fill level.
            // "Below" means "towards the bottom", i.e. in the opposite direction of fillDir?
            // No, we want to keep the filled part.
            // The filled part starts at Bottom and goes up to Level.
            // Bottom is at projection -size (relative to center along fillDir).
            // Top is at projection +size.
            // We keep points with projection <= limit.

            float limit = -size + hpPercentage * 2.0f * size;

            std::vector<Vector2> clippedVertices;
            Vector2 center = {x, y};

            // Sutherland-Hodgman clipping against the line defined by fillDir and limit
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

    DrawPolyLinesEx(Vector2{x, y}, sides, size, rotation, 3.0f, RED);
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