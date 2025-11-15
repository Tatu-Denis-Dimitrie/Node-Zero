#include "HUD.h"

void HUD::DrawDebugInfo(int posX, int posY)
{
    DrawFPS(posX, posY);
}

void HUD::DrawTitle(const char* title, int posX, int posY, int fontSize, Color color)
{
    DrawText(title, posX, posY, fontSize, color);
}
