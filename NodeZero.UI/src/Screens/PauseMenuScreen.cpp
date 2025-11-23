#include "../../include/Screens/PauseMenuScreen.h"

#include "../../include/Widgets/Button.h"
#include "../../include/Widgets/Label.h"
#include "Config/GameConfig.h"

PauseMenuScreen::PauseMenuScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback) {
    m_Menu = std::make_unique<Menu>();

    const int screenWidth = static_cast<int>(GameConfig::DEFAULT_SCREEN_WIDTH);
    const int screenHeight = static_cast<int>(GameConfig::DEFAULT_SCREEN_HEIGHT);
    const float buttonWidth = 300.0f;
    const float buttonHeight = 60.0f;
    const float buttonSpacing = 20.0f;
    float startY = screenHeight / 2.0f - 50.0f;
    float centerX = screenWidth / 2.0f - buttonWidth / 2.0f;

    auto pauseTitle = std::make_unique<Label>(
        screenWidth / 2.0f - 150.0f,
        screenHeight / 2.0f - 150.0f,
        "GAME PAUSED",
        font,
        40,
        WHITE);
    m_Menu->AddWidget(std::move(pauseTitle));

    auto resumeButton = std::make_unique<Button>(centerX, startY, buttonWidth, buttonHeight, "Resume", font);
    resumeButton->SetColors(Color{50, 150, 50, 255}, Color{80, 180, 80, 255}, Color{30, 120, 30, 255}, WHITE);
    resumeButton->SetOnClick([this]() { m_StateChangeCallback(GameScreen::Playing); });
    m_Menu->AddWidget(std::move(resumeButton));

    auto mainMenuButton = std::make_unique<Button>(centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight, "Main Menu", font);
    mainMenuButton->SetColors(Color{70, 130, 180, 255}, Color{100, 160, 210, 255}, Color{40, 100, 150, 255}, WHITE);
    mainMenuButton->SetOnClick([this]() {
        m_Game.Reset();
        m_StateChangeCallback(GameScreen::MainMenu);
    });
    m_Menu->AddWidget(std::move(mainMenuButton));
}

void PauseMenuScreen::Update(float deltaTime) {
    m_Menu->Update();
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameScreen::Playing);
    }
}

void PauseMenuScreen::Draw() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 150});
    m_Menu->Draw();
}
