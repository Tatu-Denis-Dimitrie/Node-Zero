#include "../include/MenuFactory.h"
#include "../include/Widgets/Button.h"
#include "../include/Widgets/Label.h"
#include "Config/GameConfig.h"
#include "raylib.h"

std::unique_ptr<Menu> MenuFactory::CreateMainMenu(GameState& currentState) {
    auto menu = std::make_unique<Menu>();

    const int screenWidth = static_cast<int>(GameConfig::DEFAULT_SCREEN_WIDTH);
    const int screenHeight = static_cast<int>(GameConfig::DEFAULT_SCREEN_HEIGHT);

    const float buttonWidth = 300.0f;
    const float buttonHeight = 60.0f;
    const float buttonSpacing = 20.0f;
    const float startY = screenHeight / 2.0f - 100.0f;
    const float centerX = screenWidth / 2.0f - buttonWidth / 2.0f;

    // Title Label
    auto titleLabel = std::make_unique<Label>(
        screenWidth / 2.0f - 150.0f,
        screenHeight / 2.0f - 200.0f,
        "NodeZero",
        40,
        WHITE
    );
    menu->AddWidget(std::move(titleLabel));

    // Play Button
    auto playButton = std::make_unique<Button>(
        centerX,
        startY,
        buttonWidth,
        buttonHeight,
        "Play"
    );
    playButton->SetColors(
        Color{70, 130, 180, 255},  // Steel Blue
        Color{100, 160, 210, 255}, // Lighter Blue
        Color{40, 100, 150, 255},  // Darker Blue
        WHITE
    );
    playButton->SetOnClick([&currentState]() {
        currentState = GameState::Playing;
    });
    menu->AddWidget(std::move(playButton));

    // Upgrades Button
    auto upgradesButton = std::make_unique<Button>(
        centerX,
        startY + buttonHeight + buttonSpacing,
        buttonWidth,
        buttonHeight,
        "Upgrades"
    );
    upgradesButton->SetColors(
        Color{50, 150, 50, 255},   // Green
        Color{80, 180, 80, 255},   // Lighter Green
        Color{30, 120, 30, 255},   // Darker Green
        WHITE
    );
    upgradesButton->SetOnClick([&currentState]() {
        currentState = GameState::Settings; // Using Settings state for Upgrades
    });
    menu->AddWidget(std::move(upgradesButton));

    // Quit Button
    auto quitButton = std::make_unique<Button>(
        centerX,
        startY + (buttonHeight + buttonSpacing) * 2,
        buttonWidth,
        buttonHeight,
        "Quit"
    );
    quitButton->SetColors(
        Color{180, 70, 70, 255},   // Red
        Color{210, 100, 100, 255}, // Lighter Red
        Color{150, 40, 40, 255},   // Darker Red
        WHITE
    );
    quitButton->SetOnClick([&currentState]() {
        currentState = GameState::Quit;
    });
    menu->AddWidget(std::move(quitButton));

    return menu;
}
