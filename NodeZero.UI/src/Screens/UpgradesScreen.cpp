#include "Screens/UpgradesScreen.h"

#include <cstdio>

UpgradesScreen::UpgradesScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback), m_WasMousePressed(false), m_IsFirstFrame(true), m_Font(font) {
}

void UpgradesScreen::Update(float deltaTime) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameScreen::MainMenu);
        m_IsFirstFrame = true;
    }
}

void UpgradesScreen::Draw() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int titleFontSize = static_cast<int>(screenHeight * 0.05f);
    Vector2 titleSize = MeasureTextEx(m_Font, "UPGRADES", static_cast<float>(titleFontSize), 1);
    DrawTextEx(m_Font, "UPGRADES", Vector2{static_cast<float>(screenWidth / 2 - titleSize.x / 2), static_cast<float>(screenHeight * 0.05f)}, static_cast<float>(titleFontSize), 1, WHITE);

    SaveData saveData = m_Game.GetSaveData();

    int statsX = static_cast<int>(screenWidth * 0.03f);
    int statsY = static_cast<int>(screenHeight * 0.15f);
    int statsFontSize = static_cast<int>(screenHeight * 0.035f);
    int statsSpacing = static_cast<int>(screenHeight * 0.04f);

    int subtitleFontSize = static_cast<int>(screenHeight * 0.035f);
    DrawTextEx(m_Font, "STATISTICS", Vector2{static_cast<float>(statsX), static_cast<float>(statsY)}, static_cast<float>(subtitleFontSize), 1, YELLOW);

    int statTextFontSize = static_cast<int>(screenHeight * 0.025f);
    char pointsText[64];
    snprintf(pointsText, sizeof(pointsText), "Points: %d", saveData.points);
    DrawTextEx(m_Font, pointsText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + statsSpacing)}, static_cast<float>(statTextFontSize), 1, Color{255, 215, 0, 255});

    char highPointsText[64];
    snprintf(highPointsText, sizeof(highPointsText), "High Points: %d", saveData.highPoints);
    DrawTextEx(m_Font, highPointsText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + statsSpacing * 2)}, static_cast<float>(statTextFontSize), 1, WHITE);

    char gamesPlayedText[64];
    snprintf(gamesPlayedText, sizeof(gamesPlayedText), "Games Played: %d", saveData.gamesPlayed);
    DrawTextEx(m_Font, gamesPlayedText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + statsSpacing * 3)}, static_cast<float>(statTextFontSize), 1, WHITE);

    char totalNodesText[64];
    snprintf(totalNodesText, sizeof(totalNodesText), "Total Nodes: %d", saveData.totalNodesDestroyed);
    DrawTextEx(m_Font, totalNodesText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + statsSpacing * 4)}, static_cast<float>(statTextFontSize), 1, WHITE);

    char maxHealthText[64];
    snprintf(maxHealthText, sizeof(maxHealthText), "Max Health: %.0f", m_Game.GetMaxHealth());
    DrawTextEx(m_Font, maxHealthText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + statsSpacing * 5)}, static_cast<float>(statTextFontSize), 1, WHITE);

    char regenRateText[64];
    snprintf(regenRateText, sizeof(regenRateText), "Regen Rate: %.1f/s", m_Game.GetRegenRate());
    DrawTextEx(m_Font, regenRateText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + statsSpacing * 6)}, static_cast<float>(statTextFontSize), 1, WHITE);

    char damageZoneSizeText[64];
    snprintf(damageZoneSizeText, sizeof(damageZoneSizeText), "Damage Zone: %.0f", m_Game.GetDamageZoneSize());
    DrawTextEx(m_Font, damageZoneSizeText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + statsSpacing * 7)}, static_cast<float>(statTextFontSize), 1, WHITE);

    char damagePerTickText[64];
    snprintf(damagePerTickText, sizeof(damagePerTickText), "Damage/Tick: %.0f", m_Game.GetDamagePerTick());
    DrawTextEx(m_Font, damagePerTickText, Vector2{static_cast<float>(statsX), static_cast<float>(statsY + statsSpacing * 8)}, static_cast<float>(statTextFontSize), 1, WHITE);

    int upgradeY = screenHeight / 2 - static_cast<int>(screenHeight * 0.1f);
    int upgradeTitleFontSize = static_cast<int>(screenHeight * 0.038f);
    Vector2 upgradesTitleSize = MeasureTextEx(m_Font, "UPGRADES", static_cast<float>(upgradeTitleFontSize), 1);
    DrawTextEx(m_Font, "UPGRADES", Vector2{static_cast<float>(screenWidth / 2 - upgradesTitleSize.x / 2), static_cast<float>(upgradeY - screenHeight * 0.05f)}, static_cast<float>(upgradeTitleFontSize), 1, Color{100, 200, 255, 255});

    int buttonWidth = static_cast<int>(screenWidth * 0.2f);
    int buttonHeight = static_cast<int>(screenHeight * 0.07f);
    int buttonX = screenWidth / 2 - buttonWidth / 2;
    int buttonY = upgradeY + static_cast<int>(screenHeight * 0.01f);

    Rectangle upgradeButton = {static_cast<float>(buttonX), static_cast<float>(buttonY),
                               static_cast<float>(buttonWidth), static_cast<float>(buttonHeight)};
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, upgradeButton);

    bool canAfford = saveData.points >= m_Game.GetHealthUpgradeCost();
    Color buttonColor = canAfford ? (isHovered ? Color{80, 180, 80, 255} : Color{60, 160, 60, 255})
                                  : Color{100, 100, 100, 255};

    DrawRectangleRec(upgradeButton, buttonColor);
    DrawRectangleLinesEx(upgradeButton, 1, WHITE);

    int buttonTextFontSize = static_cast<int>(buttonHeight * 0.35f);
    int costTextFontSize = static_cast<int>(buttonHeight * 0.28f);

    char buttonText[64];
    snprintf(buttonText, sizeof(buttonText), "Upgrade +1.0 HP");
    Vector2 buttonTextSize = MeasureTextEx(m_Font, buttonText, static_cast<float>(buttonTextFontSize), 1);
    DrawTextEx(m_Font, buttonText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - buttonTextSize.x / 2), static_cast<float>(buttonY + buttonHeight * 0.15f)}, static_cast<float>(buttonTextFontSize), 1, WHITE);

    char costText[32];
    snprintf(costText, sizeof(costText), "Cost: %d points", m_Game.GetHealthUpgradeCost());
    Vector2 costTextSize = MeasureTextEx(m_Font, costText, static_cast<float>(costTextFontSize), 1);
    DrawTextEx(m_Font, costText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - costTextSize.x / 2), static_cast<float>(buttonY + buttonHeight * 0.55f)}, static_cast<float>(costTextFontSize), 1, LIGHTGRAY);

    bool isMousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    if (!m_IsFirstFrame && isHovered && canAfford && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyHealthUpgrade()) {
            saveData = m_Game.GetSaveData();
        }
    }

    int buttonSpacing = static_cast<int>(screenHeight * 0.01f);
    int regenButtonY = buttonY + buttonHeight + buttonSpacing;
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
    Vector2 regenButtonTextSize = MeasureTextEx(m_Font, regenButtonText, static_cast<float>(buttonTextFontSize), 1);
    DrawTextEx(m_Font, regenButtonText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - regenButtonTextSize.x / 2), static_cast<float>(regenButtonY + buttonHeight * 0.15f)}, static_cast<float>(buttonTextFontSize), 1, WHITE);

    char regenCostText[32];
    snprintf(regenCostText, sizeof(regenCostText), "Cost: %d points", m_Game.GetRegenUpgradeCost());
    Vector2 regenCostTextSize = MeasureTextEx(m_Font, regenCostText, static_cast<float>(costTextFontSize), 1);
    DrawTextEx(m_Font, regenCostText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - regenCostTextSize.x / 2), static_cast<float>(regenButtonY + buttonHeight * 0.55f)}, static_cast<float>(costTextFontSize), 1, LIGHTGRAY);

    if (!m_IsFirstFrame && isRegenHovered && canAffordRegen && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyRegenUpgrade()) {
            saveData = m_Game.GetSaveData();
        }
    }

    int damageZoneButtonY = regenButtonY + buttonHeight + buttonSpacing;
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
    Vector2 damageZoneButtonTextSize = MeasureTextEx(m_Font, damageZoneButtonText, static_cast<float>(buttonTextFontSize), 1);
    DrawTextEx(m_Font, damageZoneButtonText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - damageZoneButtonTextSize.x / 2), static_cast<float>(damageZoneButtonY + buttonHeight * 0.15f)}, static_cast<float>(buttonTextFontSize), 1, WHITE);

    char damageZoneCostText[32];
    snprintf(damageZoneCostText, sizeof(damageZoneCostText), "Cost: %d points", m_Game.GetDamageZoneUpgradeCost());
    Vector2 damageZoneCostTextSize = MeasureTextEx(m_Font, damageZoneCostText, static_cast<float>(costTextFontSize), 1);
    DrawTextEx(m_Font, damageZoneCostText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - damageZoneCostTextSize.x / 2), static_cast<float>(damageZoneButtonY + buttonHeight * 0.55f)}, static_cast<float>(costTextFontSize), 1, LIGHTGRAY);

    if (!m_IsFirstFrame && isDamageZoneHovered && canAffordDamageZone && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyDamageZoneUpgrade()) {
            saveData = m_Game.GetSaveData();
        }
    }

    int damageButtonY = damageZoneButtonY + buttonHeight + buttonSpacing;
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
    Vector2 damageButtonTextSize = MeasureTextEx(m_Font, damageButtonText, static_cast<float>(buttonTextFontSize), 1);
    DrawTextEx(m_Font, damageButtonText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - damageButtonTextSize.x / 2), static_cast<float>(damageButtonY + buttonHeight * 0.15f)}, static_cast<float>(buttonTextFontSize), 1, WHITE);

    char damageCostText[32];
    snprintf(damageCostText, sizeof(damageCostText), "Cost: %d points", m_Game.GetDamageUpgradeCost());
    Vector2 damageCostTextSize = MeasureTextEx(m_Font, damageCostText, static_cast<float>(costTextFontSize), 1);
    DrawTextEx(m_Font, damageCostText, Vector2{static_cast<float>(buttonX + buttonWidth / 2 - damageCostTextSize.x / 2), static_cast<float>(damageButtonY + buttonHeight * 0.55f)}, static_cast<float>(costTextFontSize), 1, LIGHTGRAY);

    if (!m_IsFirstFrame && isDamageHovered && canAffordDamage && isMousePressed && !m_WasMousePressed) {
        if (m_Game.BuyDamageUpgrade()) {
            saveData = m_Game.GetSaveData();
        }
    }

    m_WasMousePressed = isMousePressed;
    m_IsFirstFrame = false;

    int escTextFontSize = static_cast<int>(screenHeight * 0.025f);
    Vector2 escTextSize = MeasureTextEx(m_Font, "Press ESC to return to menu", static_cast<float>(escTextFontSize), 1);
    DrawTextEx(m_Font, "Press ESC to return to menu", Vector2{static_cast<float>(screenWidth / 2 - escTextSize.x / 2), static_cast<float>(screenHeight - screenHeight * 0.05f)}, static_cast<float>(escTextFontSize), 1, LIGHTGRAY);
}
