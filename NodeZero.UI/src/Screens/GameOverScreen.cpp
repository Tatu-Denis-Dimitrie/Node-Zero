#include "../../include/Screens/GameOverScreen.h"

#include <string>

#include "../../include/Widgets/Button.h"
#include "../../include/Widgets/Label.h"
#include "Config/GameConfig.h"

GameOverScreen::GameOverScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
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
        screenWidth / 2.0f - 150.0f,
        screenHeight / 2.0f - 150.0f,
        "GAME OVER",
        font,
        60,
        RED);
    m_Menu->AddWidget(std::move(title));

    auto restartButton = std::make_unique<Button>(centerX, startY, buttonWidth, buttonHeight, "Restart Level", font);
    restartButton->SetColors(Color{50, 150, 50, 255}, Color{80, 180, 80, 255}, Color{30, 120, 30, 255}, WHITE);
    restartButton->SetOnClick([this]() {
        m_Game.Reset();
        m_StateChangeCallback(GameScreen::Playing);
    });
    m_Menu->AddWidget(std::move(restartButton));

    auto menuButton = std::make_unique<Button>(centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight, "Main Menu", font);
    menuButton->SetColors(Color{150, 50, 50, 255}, Color{180, 80, 80, 255}, Color{120, 30, 30, 255}, WHITE);
    menuButton->SetOnClick([this]() {
        m_Game.Reset();
        m_StateChangeCallback(GameScreen::MainMenu);
    });
    m_Menu->AddWidget(std::move(menuButton));
}

void GameOverScreen::Update(float deltaTime) {
    m_Menu->Update();
}

void GameOverScreen::Draw() {
    // Draw a semi-transparent background over the game
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 200});
    m_Menu->Draw();

    // Draw Score
    std::string scoreText = "Score: " + std::to_string(m_Game.GetPickupScore());
    Vector2 textSize = MeasureTextEx(m_Font, scoreText.c_str(), 30, 1);
    DrawTextEx(m_Font, scoreText.c_str(),
               Vector2{GetScreenWidth() / 2.0f - textSize.x / 2.0f, GetScreenHeight() / 2.0f - 80.0f},
               30, 1, WHITE);
}
