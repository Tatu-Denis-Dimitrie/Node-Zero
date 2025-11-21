#include "../include/UI.h"
#include <string>

void UI::DrawDebugInfo(int posX, int posY)
{
    DrawFPS(posX, posY);
}

void UI::DrawTitle(const char* title, int posX, int posY, int fontSize, Color color)
{
    DrawText(title, posX, posY, fontSize, color);
}

void UI::DrawScore(int score, int posX, int posY, int fontSize, Color color)
{
    std::string scoreText = "Score: " + std::to_string(score);
    DrawText(scoreText.c_str(), posX, posY, fontSize, color);
}

void UI::DrawMultiplier(int multiplier, int posX, int posY, int fontSize, Color color)
{
    if (multiplier > 1)
    {
        std::string multiplierText = "x" + std::to_string(multiplier);
        DrawText(multiplierText.c_str(), posX, posY, fontSize, color);
    }
}

void UI::DrawHealthBar(float health, float maxHealth, int posX, int posY, int width, int height)
{
    DrawRectangle(posX, posY, width, height, Color{60, 60, 60, 255});

    float healthPercentage = health / maxHealth;
    if (healthPercentage < 0.0f) healthPercentage = 0.0f;
    if (healthPercentage > 1.0f) healthPercentage = 1.0f;

    int barWidth = static_cast<int>(width * healthPercentage);

    Color barColor;
    if (healthPercentage > 0.6f) {
        barColor = Color{0, 255, 100, 255}; // Green
    } else if (healthPercentage > 0.3f) {
        barColor = Color{255, 200, 0, 255}; // Yellow
    } else {
        barColor = Color{255, 50, 50, 255}; // Red
    }

    DrawRectangle(posX, posY, barWidth, height, barColor);

    DrawRectangleLines(posX, posY, width, height, WHITE);

    char healthBuffer[32];
    snprintf(healthBuffer, sizeof(healthBuffer), "%.1f / %.0f", health, maxHealth);
    DrawText(healthBuffer, posX + 5, posY + 2, 16, WHITE);
}
