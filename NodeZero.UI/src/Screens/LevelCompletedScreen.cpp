#include "../../include/Screens/LevelCompletedScreen.h"

#include "../../include/Widgets/Button.h"
#include "../../include/Widgets/Label.h"
#include "Config/GameConfig.h"

LevelCompletedScreen::LevelCompletedScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback), m_Font(font) {
    m_Menu = std::make_unique<Menu>();

    const int screenWidth = static_cast<int>(GameConfig::DEFAULT_SCREEN_WIDTH);
    const int screenHeight = static_cast<int>(GameConfig::DEFAULT_SCREEN_HEIGHT);
    const float buttonWidth = 300.0f;
    const float buttonHeight = 60.0f;
    const float buttonSpacing = 20.0f;
    float startY = screenHeight / 2.0f;
    float centerX = screenWidth / 2.0f - buttonWidth / 2.0f;

    auto title = std::make_unique<Label>(
        screenWidth / 2.0f - 200.0f,
        screenHeight / 2.0f - 150.0f,
        "LEVEL COMPLETED!",
        font,
        40,
        GOLD);
    m_Menu->AddWidget(std::move(title));

    auto continueButton = std::make_unique<Button>(centerX, startY, buttonWidth, buttonHeight, "Continue", font);
    continueButton->SetColors(Color{50, 150, 50, 255}, Color{80, 180, 80, 255}, Color{30, 120, 30, 255}, WHITE);
    continueButton->SetOnClick([this]() {
        m_Game.StartNextLevel();
        m_StateChangeCallback(GameScreen::MainMenu);
    });
    m_Menu->AddWidget(std::move(continueButton));
}

void LevelCompletedScreen::Update(float deltaTime) {
    m_Menu->Update();
}

void LevelCompletedScreen::Draw() {
    // Draw a semi-transparent background over the game
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 200});
    m_Menu->Draw();
}
