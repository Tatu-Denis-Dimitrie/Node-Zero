#include "../../include/States/SettingsState.h"

#include <cstdio>

#include "Systems/SaveSystem.h"
#include "Types/SaveData.h"

SettingsState::SettingsState(IGame& game, std::function<void(GameState)> stateChangeCallback)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback), m_WasMousePressed(false), m_IsFirstFrame(true) {
}

void SettingsState::Update(float deltaTime) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameState::MainMenu);
        m_IsFirstFrame = true;
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

    char coinsText[64];
    snprintf(coinsText, sizeof(coinsText), "Coins: %d", saveData.coins);
    DrawText(coinsText, statsX, statsY + 50, 22, Color{255, 215, 0, 255});

    char highScoreText[64];
    snprintf(highScoreText, sizeof(highScoreText), "High Score: %d", saveData.highScore);
    DrawText(highScoreText, statsX, statsY + 85, 22, WHITE);

    char gamesPlayedText[64];
    snprintf(gamesPlayedText, sizeof(gamesPlayedText), "Games Played: %d", saveData.gamesPlayed);
    DrawText(gamesPlayedText, statsX, statsY + 120, 22, WHITE);

    char totalNodesText[64];
    snprintf(totalNodesText, sizeof(totalNodesText), "Total Nodes: %d", saveData.totalNodesDestroyed);
    DrawText(totalNodesText, statsX, statsY + 155, 22, WHITE);

    char maxHealthText[64];
    snprintf(maxHealthText, sizeof(maxHealthText), "Max Health: %.0f", m_Game.GetMaxHealth());
    DrawText(maxHealthText, statsX, statsY + 190, 22, WHITE);

    char damageZoneSizeText[64];
    snprintf(damageZoneSizeText, sizeof(damageZoneSizeText), "Damage Zone: %.0f", m_Game.GetDamageZoneSize());
    DrawText(damageZoneSizeText, statsX, statsY + 225, 22, WHITE);

    char damagePerTickText[64];
    snprintf(damagePerTickText, sizeof(damagePerTickText), "Damage/Tick: %.0f", m_Game.GetDamagePerTick());
    DrawText(damagePerTickText, statsX, statsY + 260, 22, WHITE);

    int upgradeY = screenHeight / 2 - 80;
    DrawText("UPGRADES", screenWidth / 2 - 80, upgradeY - 40, 30, Color{100, 200, 255, 255});

    int buttonX = screenWidth / 2 - 140;
    int buttonY = upgradeY + 10;
    int buttonWidth = 280;
    int buttonHeight = 50;

    Rectangle upgradeButton = {static_cast<float>(buttonX), static_cast<float>(buttonY),
                               static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, upgradeButton);

    bool canAfford = saveData.coins >= m_Game.GetHealthUpgradeCost();
    Color buttonColor = canAfford ? (isHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                  : Color{100, 100, 100, 255};

    DrawRectangleRec(upgradeButton, buttonColor);
    DrawRectangleLinesEx(upgradeButton, 2, WHITE);

    char buttonText[64];
    snprintf(buttonText, sizeof(buttonText), "Upgrade +1.0 HP", m_Game.GetHealthUpgradeCost());
    DrawText(buttonText, buttonX + 65, buttonY + 8, 22, WHITE);

    char costText[32];
    snprintf(costText, sizeof(costText), "Cost: %d coins", m_Game.GetHealthUpgradeCost());
    DrawText(costText, buttonX + 75, buttonY + 28, 18, LIGHTGRAY);

    bool isMousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    if (!m_IsFirstFrame && isHovered && canAfford && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyHealthUpgrade()) {
            saveData = SaveSystem::LoadProgress();
        }
    }

    // Damage Zone Upgrade Button
    int damageZoneButtonY = upgradeY + 80;
    Rectangle damageZoneButton = {static_cast<float>(buttonX), static_cast<float>(damageZoneButtonY),
                                   static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    bool isDamageZoneHovered = CheckCollisionPointRec(mousePos, damageZoneButton);

    bool canAffordDamageZone = saveData.coins >= m_Game.GetDamageZoneUpgradeCost();
    Color damageZoneButtonColor = canAffordDamageZone ? (isDamageZoneHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                                       : Color{100, 100, 100, 255};

    DrawRectangleRec(damageZoneButton, damageZoneButtonColor);
    DrawRectangleLinesEx(damageZoneButton, 2, WHITE);

    char damageZoneButtonText[64];
    snprintf(damageZoneButtonText, sizeof(damageZoneButtonText), "Upgrade +10 Zone");
    DrawText(damageZoneButtonText, buttonX + 55, damageZoneButtonY + 8, 22, WHITE);

    char damageZoneCostText[32];
    snprintf(damageZoneCostText, sizeof(damageZoneCostText), "Cost: %d coins", m_Game.GetDamageZoneUpgradeCost());
    DrawText(damageZoneCostText, buttonX + 75, damageZoneButtonY + 28, 18, LIGHTGRAY);

    if (!m_IsFirstFrame && isDamageZoneHovered && canAffordDamageZone && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyDamageZoneUpgrade()) {
            saveData = SaveSystem::LoadProgress();
        }
    }

    // Damage Upgrade Button
    int damageButtonY = upgradeY + 150;
    Rectangle damageButton = {static_cast<float>(buttonX), static_cast<float>(damageButtonY),
                              static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    bool isDamageHovered = CheckCollisionPointRec(mousePos, damageButton);

    bool canAffordDamage = saveData.coins >= m_Game.GetDamageUpgradeCost();
    Color damageButtonColor = canAffordDamage ? (isDamageHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                               : Color{100, 100, 100, 255};

    DrawRectangleRec(damageButton, damageButtonColor);
    DrawRectangleLinesEx(damageButton, 2, WHITE);

    char damageButtonText[64];
    snprintf(damageButtonText, sizeof(damageButtonText), "Upgrade +5 Damage");
    DrawText(damageButtonText, buttonX + 50, damageButtonY + 8, 22, WHITE);

    char damageCostText[32];
    snprintf(damageCostText, sizeof(damageCostText), "Cost: %d coins", m_Game.GetDamageUpgradeCost());
    DrawText(damageCostText, buttonX + 75, damageButtonY + 28, 18, LIGHTGRAY);

    if (!m_IsFirstFrame && isDamageHovered && canAffordDamage && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyDamageUpgrade()) {
            saveData = SaveSystem::LoadProgress();
        }
    }

    m_WasMousePressed = isMousePressed;
    m_IsFirstFrame = false;

    DrawText("Press ESC to return to menu", screenWidth / 2 - 180, screenHeight - 50, 20, LIGHTGRAY);
}
