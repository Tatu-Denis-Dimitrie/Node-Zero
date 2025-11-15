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
