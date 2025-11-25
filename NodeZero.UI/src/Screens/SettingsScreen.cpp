#include "../../include/Screens/SettingsScreen.h"

#include <cstdio>

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

    Vector2 titleSize = MeasureTextEx(m_Font, "UPGRADES", 40, 1);
    DrawTextEx(m_Font, "UPGRADES", Vector2{static_cast<float>(screenWidth / 2 - titleSize.x / 2), 50}, 40, 1, WHITE);

    SaveData saveData = m_Game.GetSaveData();

    int statsX = 50;
    int statsY = 150;

    DrawTextEx(m_Font, "STATISTICS", Vector2{static_cast<float>(statsX), static_cast<float>(statsY)}, 28, 1, YELLOW);

    char pointsText[64];
    snprintf(pointsText, sizeof(pointsText), "Points: %d", saveData.points);
    DrawTextEx(m_Font, pointsText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 50)}, 22, 1, Color{255, 215, 0, 255});

    char highPointsText[64];
    snprintf(highPointsText, sizeof(highPointsText), "High Points: %d", saveData.highPoints);
    DrawTextEx(m_Font, highPointsText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + 85)}, 22, 1, WHITE);

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

    int upgradeY = screenHeight / 2 - 100;
    Vector2 upgradesTitleSize = MeasureTextEx(m_Font, "UPGRADES", 30, 1);
    DrawTextEx(m_Font, "UPGRADES", Vector2{static_cast<float>(screenWidth / 2 - upgradesTitleSize.x / 2), static_cast<float>(upgradeY - 40)}, 30, 1, Color{100, 200, 255, 255});

    int buttonX = screenWidth / 2 - 140;
    int buttonY = upgradeY + 10;
    int buttonWidth = 280;
    int buttonHeight = 50;

    Rectangle upgradeButton = {static_cast<float>(buttonX), static_cast<float>(buttonY),
                               static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, upgradeButton);

    bool canAfford = saveData.points >= m_Game.GetHealthUpgradeCost();
    Color buttonColor = canAfford ? (isHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                  : Color{100, 100, 100, 255};

    DrawRectangleRec(upgradeButton, buttonColor);
    DrawRectangleLinesEx(upgradeButton, 1, WHITE);

    char buttonText[64];
    snprintf(buttonText, sizeof(buttonText), "Upgrade +1.0 HP");
    Vector2 buttonTextSize = MeasureTextEx(m_Font, buttonText, 22, 1);
    DrawTextEx(m_Font, buttonText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - buttonTextSize.x / 2), static_cast<float>(buttonY + 8)}, 22, 1, WHITE);

    char costText[32];
    snprintf(costText, sizeof(costText), "Cost: %d points", m_Game.GetHealthUpgradeCost());
    Vector2 costTextSize = MeasureTextEx(m_Font, costText, 18, 1);
    DrawTextEx(m_Font, costText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - costTextSize.x / 2), static_cast<float>(buttonY + 28)}, 18, 1, LIGHTGRAY);

    bool isMousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    if (!m_IsFirstFrame && isHovered && canAfford && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyHealthUpgrade()) {
            saveData = m_Game.GetSaveData();
        }
    }

    int regenButtonY = upgradeY + 80;
    Rectangle regenButton = {static_cast<float>(buttonX), static_cast<float>(regenButtonY),
                             static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    bool isRegenHovered = CheckCollisionPointRec(mousePos, regenButton);

    bool canAffordRegen = saveData.points >= m_Game.GetRegenUpgradeCost();
    Color regenButtonColor = canAffordRegen ? (isRegenHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                            : Color{100, 100, 100, 255};

    DrawRectangleRec(regenButton, regenButtonColor);
    DrawRectangleLinesEx(regenButton, 1, WHITE);

    char regenButtonText[64];
    snprintf(regenButtonText, sizeof(regenButtonText), "Upgrade +0.1 Regen");
    Vector2 regenButtonTextSize = MeasureTextEx(m_Font, regenButtonText, 22, 1);
    DrawTextEx(m_Font, regenButtonText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - regenButtonTextSize.x / 2), static_cast<float>(regenButtonY + 8)}, 22, 1, WHITE);

    char regenCostText[32];
    snprintf(regenCostText, sizeof(regenCostText), "Cost: %d points", m_Game.GetRegenUpgradeCost());
    Vector2 regenCostTextSize = MeasureTextEx(m_Font, regenCostText, 18, 1);
    DrawTextEx(m_Font, regenCostText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - regenCostTextSize.x / 2), static_cast<float>(regenButtonY + 28)}, 18, 1, LIGHTGRAY);

    if (!m_IsFirstFrame && isRegenHovered && canAffordRegen && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyRegenUpgrade()) {
            saveData = m_Game.GetSaveData();
        }
    }

    int damageZoneButtonY = upgradeY + 150;
    Rectangle damageZoneButton = {static_cast<float>(buttonX), static_cast<float>(damageZoneButtonY),
                                  static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    bool isDamageZoneHovered = CheckCollisionPointRec(mousePos, damageZoneButton);

    bool canAffordDamageZone = saveData.points >= m_Game.GetDamageZoneUpgradeCost();
    Color damageZoneButtonColor = canAffordDamageZone ? (isDamageZoneHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                                      : Color{100, 100, 100, 255};

    DrawRectangleRec(damageZoneButton, damageZoneButtonColor);
    DrawRectangleLinesEx(damageZoneButton, 1, WHITE);

    char damageZoneButtonText[64];
    snprintf(damageZoneButtonText, sizeof(damageZoneButtonText), "Upgrade +10 Zone");
    Vector2 damageZoneButtonTextSize = MeasureTextEx(m_Font, damageZoneButtonText, 22, 1);
    DrawTextEx(m_Font, damageZoneButtonText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - damageZoneButtonTextSize.x / 2), static_cast<float>(damageZoneButtonY + 8)}, 22, 1, WHITE);

    char damageZoneCostText[32];
    snprintf(damageZoneCostText, sizeof(damageZoneCostText), "Cost: %d points", m_Game.GetDamageZoneUpgradeCost());
    Vector2 damageZoneCostTextSize = MeasureTextEx(m_Font, damageZoneCostText, 18, 1);
    DrawTextEx(m_Font, damageZoneCostText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - damageZoneCostTextSize.x / 2), static_cast<float>(damageZoneButtonY + 28)}, 18, 1, LIGHTGRAY);

    if (!m_IsFirstFrame && isDamageZoneHovered && canAffordDamageZone && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyDamageZoneUpgrade()) {
            saveData = m_Game.GetSaveData();
        }
    }

    int damageButtonY = upgradeY + 220;
    Rectangle damageButton = {static_cast<float>(buttonX), static_cast<float>(damageButtonY),
                              static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    bool isDamageHovered = CheckCollisionPointRec(mousePos, damageButton);

    bool canAffordDamage = saveData.points >= m_Game.GetDamageUpgradeCost();
    Color damageButtonColor = canAffordDamage ? (isDamageHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                              : Color{100, 100, 100, 255};

    DrawRectangleRec(damageButton, damageButtonColor);
    DrawRectangleLinesEx(damageButton, 1, WHITE);

    char damageButtonText[64];
    snprintf(damageButtonText, sizeof(damageButtonText), "Upgrade +5 Damage");
    Vector2 damageButtonTextSize = MeasureTextEx(m_Font, damageButtonText, 22, 1);
    DrawTextEx(m_Font, damageButtonText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - damageButtonTextSize.x / 2), static_cast<float>(damageButtonY + 8)}, 22, 1, WHITE);

    char damageCostText[32];
    snprintf(damageCostText, sizeof(damageCostText), "Cost: %d points", m_Game.GetDamageUpgradeCost());
    Vector2 damageCostTextSize = MeasureTextEx(m_Font, damageCostText, 18, 1);
    DrawTextEx(m_Font, damageCostText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - damageCostTextSize.x / 2), static_cast<float>(damageButtonY + 28)}, 18, 1, LIGHTGRAY);

    if (!m_IsFirstFrame && isDamageHovered && canAffordDamage && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyDamageUpgrade()) {
            saveData = m_Game.GetSaveData();
        }
    }

    m_WasMousePressed = isMousePressed;
    m_IsFirstFrame = false;

    Vector2 escTextSize = MeasureTextEx(m_Font, "Press ESC to return to menu", 20, 1);
    DrawTextEx(m_Font, "Press ESC to return to menu", Vector2{static_cast<float>(screenWidth / 2 - escTextSize.x / 2), static_cast<float>(screenHeight - 50)}, 20, 1, LIGHTGRAY);
}
