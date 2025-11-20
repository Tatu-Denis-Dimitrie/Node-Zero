#include "../../include/States/SettingsState.h"

#include <cstdio>

#include "Systems/SaveSystem.h"
#include "Types/SaveData.h"

SettingsState::SettingsState(IGame& game, std::function<void(GameState)> stateChangeCallback)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback) {
}

void SettingsState::Update(float deltaTime) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameState::MainMenu);
    }
}

void SettingsState::Draw() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawText("UPGRADES", screenWidth / 2 - 100, 50, 40, WHITE);

    SaveData saveData = SaveSystem::LoadProgress();

    int statsX = 50;
    int statsY = 150;

    DrawText("STATISTICS", statsX, statsY, 28, YELLOW);

    char highScoreText[64];
    snprintf(highScoreText, sizeof(highScoreText), "High Score: %d", saveData.highScore);
    DrawText(highScoreText, statsX, statsY + 50, 22, WHITE);

    char gamesPlayedText[64];
    snprintf(gamesPlayedText, sizeof(gamesPlayedText), "Games Played: %d", saveData.gamesPlayed);
    DrawText(gamesPlayedText, statsX, statsY + 85, 22, WHITE);

    char totalNodesText[64];
    snprintf(totalNodesText, sizeof(totalNodesText), "Total Nodes: %d", saveData.totalNodesDestroyed);
    DrawText(totalNodesText, statsX, statsY + 120, 22, WHITE);

    char maxHealthText[64];
    snprintf(maxHealthText, sizeof(maxHealthText), "Max Health: %.0f", m_Game.GetMaxHealth());
    DrawText(maxHealthText, statsX, statsY + 155, 22, WHITE);

    int upgradeY = screenHeight / 2 - 50;
    DrawText("HEALTH UPGRADE", screenWidth / 2 - 120, upgradeY - 40, 30, Color{100, 200, 255, 255});

    int buttonX = screenWidth / 2 - 140;
    int buttonY = upgradeY + 10;
    int buttonWidth = 280;
    int buttonHeight = 50;

    Rectangle upgradeButton = {static_cast<float>(buttonX), static_cast<float>(buttonY),
                               static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, upgradeButton);

    bool canAfford = saveData.highScore >= m_Game.GetHealthUpgradeCost();
    Color buttonColor = canAfford ? (isHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                  : Color{100, 100, 100, 255};

    DrawRectangleRec(upgradeButton, buttonColor);
    DrawRectangleLinesEx(upgradeButton, 2, WHITE);

    char buttonText[64];
    snprintf(buttonText, sizeof(buttonText), "Upgrade +1.0 HP", m_Game.GetHealthUpgradeCost());
    DrawText(buttonText, buttonX + 65, buttonY + 8, 22, WHITE);

    char costText[32];
    snprintf(costText, sizeof(costText), "Cost: %d points", m_Game.GetHealthUpgradeCost());
    DrawText(costText, buttonX + 70, buttonY + 28, 18, LIGHTGRAY);

    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (m_Game.BuyHealthUpgrade()) {
            saveData = SaveSystem::LoadProgress();
        }
    }

    DrawText("Press ESC to return to menu", screenWidth / 2 - 180, screenHeight - 50, 20, LIGHTGRAY);
}
