#include "Screens/MainScreen.h"

#include "Config/GameConfig.h"
#include "Widgets/Button.h"
#include "Widgets/Label.h"

MainScreen::MainScreen(std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_StateChangeCallback(stateChangeCallback), m_Font(font) {
    m_Menu = std::make_unique<Menu>();

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const float buttonWidth = screenWidth * 0.25f;
    const float buttonHeight = screenHeight * 0.08f;
    const float buttonSpacing = screenHeight * 0.02f;
    float startY = screenHeight / 2.0f - buttonHeight;
    float centerX = screenWidth / 2.0f - buttonWidth / 2.0f;

    const char* titleText = "NodeZero";
    int fontSize = static_cast<int>(screenHeight * 0.06f);
    Vector2 textSize = MeasureTextEx(font, titleText, static_cast<float>(fontSize), 1);
    float titleX = screenWidth / 2.0f - textSize.x / 2.0f;

    auto titleLabel = std::make_unique<Label>(
        titleX,
        screenHeight * 0.3f,
        titleText,
        font,
        fontSize,
        WHITE);
    m_Menu->AddWidget(std::move(titleLabel));

    auto playButton = std::make_unique<Button>(centerX, startY, buttonWidth, buttonHeight, "Play", font);
    playButton->SetColors(Color{70, 130, 180, 255}, Color{100, 160, 210, 255}, Color{40, 100, 150, 255}, WHITE);
    playButton->SetOnClick([this]() { m_StateChangeCallback(GameScreen::Playing); });
    m_Menu->AddWidget(std::move(playButton));

    auto upgradesButton = std::make_unique<Button>(centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight, "Upgrades", font);
    upgradesButton->SetColors(Color{50, 150, 50, 255}, Color{80, 180, 80, 255}, Color{30, 120, 30, 255}, WHITE);
    upgradesButton->SetOnClick([this]() { m_StateChangeCallback(GameScreen::Settings); });
    m_Menu->AddWidget(std::move(upgradesButton));

    auto quitButton = std::make_unique<Button>(centerX, startY + (buttonHeight + buttonSpacing) * 2, buttonWidth, buttonHeight, "Quit", font);
    quitButton->SetColors(Color{180, 70, 70, 255}, Color{210, 100, 100, 255}, Color{150, 40, 40, 255}, WHITE);
    quitButton->SetOnClick([this]() { m_StateChangeCallback(GameScreen::Quit); });
    m_Menu->AddWidget(std::move(quitButton));
}

void MainScreen::Update(float deltaTime) {
    m_Menu->Update();
}

void MainScreen::Draw() {
    m_Menu->Draw();

    const char* watermarkText = "Made by TeamTBD";
    int watermarkFontSize = static_cast<int>(GetScreenHeight() * 0.025f);
    DrawTextEx(m_Font, watermarkText, Vector2{static_cast<float>(GetScreenWidth() * 0.01f), static_cast<float>(GetScreenHeight() * 0.96f)}, static_cast<float>(watermarkFontSize), 1, Color{150, 150, 150, 200});
}
