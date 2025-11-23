#include "../../include/Screens/SettingsScreen.h"

#include <cstdio>

#include "Systems/SaveSystem.h"
#include "Types/SaveData.h"

SettingsScreen::SettingsScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback), m_WasMousePressed(false), m_IsFirstFrame(true), m_Font(font) {
}

void SettingsScreen::Update(float deltaTime) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameScreen::MainMenu);
        m_IsFirstFrame = true;
    }
}

void SettingsScreen::Draw() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawTextEx(m_Font, "UPGRADES", Vector2{static_cast<float>(screenWidth / 2 - 100), 50}, 40, 1, WHITE);

    SaveData saveData = SaveSystem::LoadProgress();

    int statsX = 50;
    int statsY = 150;

    DrawTextEx(m_Font, "STATISTICS", Vector2{static_cast<float>(statsX), static_cast<float>(statsY)}, 28, 1, YELLOW);

    char coinsText[64];
    snprintf(coinsText, sizeof(coinsText), "Coins: %d", saveData.coins);
    DrawTextEx(m_Font, coinsText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 50)}, 22, 1, Color{255, 215, 0, 255});

    char highScoreText[64];
    snprintf(highScoreText, sizeof(highScoreText), "High Score: %d", saveData.highScore);
    DrawTextEx(m_Font, highScoreText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 85)}, 22, 1, WHITE);

    char gamesPlayedText[64];
    snprintf(gamesPlayedText, sizeof(gamesPlayedText), "Games Played: %d", saveData.gamesPlayed);
    DrawTextEx(m_Font, gamesPlayedText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 120)}, 22, 1, WHITE);

    char totalNodesText[64];
    snprintf(totalNodesText, sizeof(totalNodesText), "Total Nodes: %d", saveData.totalNodesDestroyed);
    DrawTextEx(m_Font, totalNodesText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 155)}, 22, 1, WHITE);

    char maxHealthText[64];
    snprintf(maxHealthText, sizeof(maxHealthText), "Max Health: %.0f", m_Game.GetMaxHealth());
    DrawTextEx(m_Font, maxHealthText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 190)}, 22, 1, WHITE);

    char regenRateText[64];
    snprintf(regenRateText, sizeof(regenRateText), "Regen Rate: %.1f/s", m_Game.GetRegenRate());
    DrawTextEx(m_Font, regenRateText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 225)}, 22, 1, WHITE);

    char damageZoneSizeText[64];
    snprintf(damageZoneSizeText, sizeof(damageZoneSizeText), "Damage Zone: %.0f", m_Game.GetDamageZoneSize());
    DrawTextEx(m_Font, damageZoneSizeText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 260)}, 22, 1, WHITE);

    char damagePerTickText[64];
    snprintf(damagePerTickText, sizeof(damagePerTickText), "Damage/Tick: %.0f", m_Game.GetDamagePerTick());
    DrawTextEx(m_Font, damagePerTickText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 295)}, 22, 1, WHITE);

    int upgradeY = screenHeight / 2 - 100; // Moved up slightly to fit more buttons
    DrawTextEx(m_Font, "UPGRADES", Vector2{static_cast<float>(screenWidth / 2 - 80), static_cast<float>(upgradeY - 40)}, 30, 1, Color{100, 200, 255, 255});

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
    DrawRectangleLinesEx(upgradeButton, 1, WHITE);

    char buttonText[64];
    snprintf(buttonText, sizeof(buttonText), "Upgrade +1.0 HP", m_Game.GetHealthUpgradeCost());
    DrawTextEx(m_Font, buttonText, Vector2{static_cast<float>(buttonX + 65), static_cast<float>(buttonY + 8)}, 22, 1, WHITE);

    char costText[32];
    snprintf(costText, sizeof(costText), "Cost: %d coins", m_Game.GetHealthUpgradeCost());
    DrawTextEx(m_Font, costText, Vector2{static_cast<float>(buttonX + 75), static_cast<float>(buttonY + 28)}, 18, 1, LIGHTGRAY);

    bool isMousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    if (!m_IsFirstFrame && isHovered && canAfford && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyHealthUpgrade()) {
            saveData = SaveSystem::LoadProgress();
        }
    }

    // Regen Upgrade Button
    int regenButtonY = upgradeY + 80;
    Rectangle regenButton = {static_cast<float>(buttonX), static_cast<float>(regenButtonY),
                             static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    bool isRegenHovered = CheckCollisionPointRec(mousePos, regenButton);

    bool canAffordRegen = saveData.coins >= m_Game.GetRegenUpgradeCost();
    Color regenButtonColor = canAffordRegen ? (isRegenHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                            : Color{100, 100, 100, 255};

    DrawRectangleRec(regenButton, regenButtonColor);
    DrawRectangleLinesEx(regenButton, 1, WHITE);

    char regenButtonText[64];
    snprintf(regenButtonText, sizeof(regenButtonText), "Upgrade +0.1 Regen");
    DrawTextEx(m_Font, regenButtonText, Vector2{static_cast<float>(buttonX + 55), static_cast<float>(regenButtonY + 8)}, 22, 1, WHITE);

    char regenCostText[32];
    snprintf(regenCostText, sizeof(regenCostText), "Cost: %d coins", m_Game.GetRegenUpgradeCost());
    DrawTextEx(m_Font, regenCostText, Vector2{static_cast<float>(buttonX + 75), static_cast<float>(regenButtonY + 28)}, 18, 1, LIGHTGRAY);

    if (!m_IsFirstFrame && isRegenHovered && canAffordRegen && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyRegenUpgrade()) {
            saveData = SaveSystem::LoadProgress();
        }
    }

    // Damage Zone Upgrade Button
    int damageZoneButtonY = upgradeY + 150;
    Rectangle damageZoneButton = {static_cast<float>(buttonX), static_cast<float>(damageZoneButtonY),
                                   static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    bool isDamageZoneHovered = CheckCollisionPointRec(mousePos, damageZoneButton);

    bool canAffordDamageZone = saveData.coins >= m_Game.GetDamageZoneUpgradeCost();
    Color damageZoneButtonColor = canAffordDamageZone ? (isDamageZoneHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                                       : Color{100, 100, 100, 255};

    DrawRectangleRec(damageZoneButton, damageZoneButtonColor);
    DrawRectangleLinesEx(damageZoneButton, 1, WHITE);

    char damageZoneButtonText[64];
    snprintf(damageZoneButtonText, sizeof(damageZoneButtonText), "Upgrade +10 Zone");
    DrawTextEx(m_Font, damageZoneButtonText, Vector2{static_cast<float>(buttonX + 55), static_cast<float>(damageZoneButtonY + 8)}, 22, 1, WHITE);

    char damageZoneCostText[32];
    snprintf(damageZoneCostText, sizeof(damageZoneCostText), "Cost: %d coins", m_Game.GetDamageZoneUpgradeCost());
    DrawTextEx(m_Font, damageZoneCostText, Vector2{static_cast<float>(buttonX + 75), static_cast<float>(damageZoneButtonY + 28)}, 18, 1, LIGHTGRAY);

    if (!m_IsFirstFrame && isDamageZoneHovered && canAffordDamageZone && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyDamageZoneUpgrade()) {
            saveData = SaveSystem::LoadProgress();
        }
    }

    // Damage Upgrade Button
    int damageButtonY = upgradeY + 220;
    Rectangle damageButton = {static_cast<float>(buttonX), static_cast<float>(damageButtonY),
                              static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    bool isDamageHovered = CheckCollisionPointRec(mousePos, damageButton);

    bool canAffordDamage = saveData.coins >= m_Game.GetDamageUpgradeCost();
    Color damageButtonColor = canAffordDamage ? (isDamageHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                               : Color{100, 100, 100, 255};

    DrawRectangleRec(damageButton, damageButtonColor);
    DrawRectangleLinesEx(damageButton, 1, WHITE);

    char damageButtonText[64];
    snprintf(damageButtonText, sizeof(damageButtonText), "Upgrade +5 Damage");
    DrawTextEx(m_Font, damageButtonText, Vector2{static_cast<float>(buttonX + 50), static_cast<float>(damageButtonY + 8)}, 22, 1, WHITE);

    char damageCostText[32];
    snprintf(damageCostText, sizeof(damageCostText), "Cost: %d coins", m_Game.GetDamageUpgradeCost());
    DrawTextEx(m_Font, damageCostText, Vector2{static_cast<float>(buttonX + 75), static_cast<float>(damageButtonY + 28)}, 18, 1, LIGHTGRAY);

    if (!m_IsFirstFrame && isDamageHovered && canAffordDamage && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyDamageUpgrade()) {
            saveData = SaveSystem::LoadProgress();
        }
    }

    m_WasMousePressed = isMousePressed;
    m_IsFirstFrame = false;

    DrawTextEx(m_Font, "Press ESC to return to menu", Vector2{static_cast<float>(screenWidth / 2 - 180), static_cast<float>(screenHeight - 50)}, 20, 1, LIGHTGRAY);
}
