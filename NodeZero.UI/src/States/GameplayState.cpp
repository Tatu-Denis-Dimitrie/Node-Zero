#include "../../include/States/GameplayState.h"

#include <algorithm>

#include "../../include/InputHandler.h"
#include "../../include/Renderer.h"
#include "../../include/UI.h"
#include "INode.h"
#include "raymath.h"

GameplayState::GameplayState(IGame& game, std::function<void(GameState)> stateChangeCallback)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback) {
}

void GameplayState::Update(float deltaTime) {
    Vector2 mousePos = InputHandler::GetMousePosition();

    m_Game.UpdateHealth(deltaTime);
    m_Game.UpdateAutoSpawn(deltaTime);
    m_Game.UpdateDamageTimer(deltaTime);

    if (m_Game.ShouldGameOver()) {
        m_Game.SaveProgress();
        m_Game.Reset();
        m_StateChangeCallback(GameState::MainMenu);
        return;
    }

    bool shouldDealDamage = m_Game.ShouldDealDamage();
    if (shouldDealDamage) {
        m_Game.ResetDamageTimer();
    }
    float damageZoneSize = m_Game.GetDamageZoneSize();
    float damagePerTick = m_Game.GetDamagePerTick();
    m_Game.ProcessDamageZone(mousePos.x, mousePos.y, damageZoneSize, damagePerTick, shouldDealDamage);

    if (m_Game.ShouldGameOver()) {
        m_Game.SaveProgress();
        m_Game.Reset();
        m_StateChangeCallback(GameState::MainMenu);
        return;
    }

    const auto& pickups = m_Game.GetPickups();
    for (const PointPickup& pickup : pickups) {
        if (pickup.GetAge() < PICKUP_COLLECT_DELAY) continue;

        float collectRectX = mousePos.x - damageZoneSize / 2.0f;
        float collectRectY = mousePos.y - damageZoneSize / 2.0f;

        bool intersects = !(pickup.position.x + pickup.size < collectRectX ||
                            pickup.position.x - pickup.size > collectRectX + damageZoneSize ||
                            pickup.position.y + pickup.size < collectRectY ||
                            pickup.position.y - pickup.size > collectRectY + damageZoneSize);

        if (intersects && m_Game.CollectPickup(pickup.id)) {
            PickupCollectEffect effect{};
            effect.startPosition = Vector2{pickup.position.x, pickup.position.y};
            effect.elapsed = 0.0f;
            effect.duration = PICKUP_COLLECT_EFFECT_DURATION;
            effect.size = pickup.size;
            m_PickupEffects.push_back(effect);
        }
    }

    for (auto& effect : m_PickupEffects) {
        effect.elapsed += deltaTime;
    }

    m_PickupEffects.erase(
        std::remove_if(m_PickupEffects.begin(), m_PickupEffects.end(),
                       [](const PickupCollectEffect& effect) { return effect.elapsed >= effect.duration; }),
        m_PickupEffects.end());

    m_Game.Update(deltaTime);

    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameState::Paused);
    }
}

void GameplayState::Draw() {
    Vector2 mousePos = InputHandler::GetMousePosition();
    float damageZoneSize = m_Game.GetDamageZoneSize();
    float damageRectX = mousePos.x - damageZoneSize / 2.0f;
    float damageRectY = mousePos.y - damageZoneSize / 2.0f;

    const auto& nodes = m_Game.GetNodes();
    for (const INode* node : nodes) {
        if (node->GetState() == NodeState::Active) {
            float x = node->GetPosition().x;
            float y = node->GetPosition().y;
            float size = node->GetSize();
            float hpPercentage = node->GetHP() / node->GetMaxHP();
            float rotation = node->GetRotation();

            switch (node->GetShape()) {
                case NodeShape::Circle:
                    Renderer::DrawCircleNode(x, y, size, hpPercentage, RED);
                    break;
                case NodeShape::Square:
                    Renderer::DrawSquareNode(x, y, size, hpPercentage, RED, rotation);
                    break;
                case NodeShape::Hexagon:
                    Renderer::DrawHexagonNode(x, y, size, hpPercentage, RED, rotation);
                    break;
                default:
                    Renderer::DrawCircleNode(x, y, size, hpPercentage, RED);
                    break;
            }
        }
    }

    const auto& pickups = m_Game.GetPickups();
    for (const PointPickup& pickup : pickups) {
        float lifeRatio = std::clamp(pickup.GetLifeRatio(), 0.0f, 1.0f);
        unsigned char alpha = static_cast<unsigned char>(lifeRatio * 255.0f);
        Color pickupColor = Color{0, 255, 150, alpha};
        Vector2 pickupPos{pickup.position.x, pickup.position.y};
        float pickupSize = pickup.size;
        float spawnAge = pickup.GetAge();
        if (spawnAge < PICKUP_SPAWN_ANIM_DURATION) {
            float t = std::clamp(spawnAge / PICKUP_SPAWN_ANIM_DURATION, 0.0f, 1.0f);
            Vector2 origin{pickup.spawnOrigin.x, pickup.spawnOrigin.y};
            Vector2 target{pickup.position.x, pickup.position.y};
            pickupPos = Vector2Lerp(origin, target, t);
            pickupSize = pickup.size * t;
            alpha = static_cast<unsigned char>(std::min(255.0f, alpha + (1.0f - t) * 100.0f));
            pickupColor = Color{0, 255, 200, alpha};
        }
        Renderer::DrawPickup(pickupPos.x, pickupPos.y, pickupSize, pickupColor);
    }

    for (const PickupCollectEffect& effect : m_PickupEffects) {
        float t = std::clamp(effect.elapsed / effect.duration, 0.0f, 1.0f);
        Vector2 targetPos{mousePos.x, mousePos.y};
        Vector2 currentPos = Vector2Lerp(effect.startPosition, targetPos, t);
        unsigned char alpha = static_cast<unsigned char>((1.0f - t) * 255.0f);
        Renderer::DrawPickup(currentPos.x, currentPos.y, effect.size, Color{0, 255, 150, alpha});
    }

    DrawRectangle(
        static_cast<int>(damageRectX),
        static_cast<int>(damageRectY),
        static_cast<int>(damageZoneSize),
        static_cast<int>(damageZoneSize),
        Color{0, 100, 255, 80});

    float cornerLength = 20.0f;
    float cornerThickness = 3.0f;
    Color cornerColor = Color{0, 200, 255, 255};

    float left = damageRectX;
    float right = mousePos.x + damageZoneSize / 2;
    float top = damageRectY;
    float bottom = mousePos.y + damageZoneSize / 2;

    DrawLineEx(Vector2{left, top}, Vector2{left + cornerLength, top}, cornerThickness, cornerColor);
    DrawLineEx(Vector2{left, top}, Vector2{left, top + cornerLength}, cornerThickness, cornerColor);
    DrawLineEx(Vector2{right, top}, Vector2{right - cornerLength, top}, cornerThickness, cornerColor);
    DrawLineEx(Vector2{right, top}, Vector2{right, top + cornerLength}, cornerThickness, cornerColor);
    DrawLineEx(Vector2{left, bottom}, Vector2{left + cornerLength, bottom}, cornerThickness, cornerColor);
    DrawLineEx(Vector2{left, bottom}, Vector2{left, bottom - cornerLength}, cornerThickness, cornerColor);
    DrawLineEx(Vector2{right, bottom}, Vector2{right - cornerLength, bottom}, cornerThickness, cornerColor);
    DrawLineEx(Vector2{right, bottom}, Vector2{right, bottom - cornerLength}, cornerThickness, cornerColor);

    float centerSquareSize = 8.0f;
    DrawRectangle(
        static_cast<int>(mousePos.x - centerSquareSize / 2),
        static_cast<int>(mousePos.y - centerSquareSize / 2),
        static_cast<int>(centerSquareSize),
        static_cast<int>(centerSquareSize),
        WHITE);

    // Draw Progress Bar
    int screenWidth = GetScreenWidth();
    int progressBarWidth = 400;
    int progressBarHeight = 30;
    int progressBarX = (screenWidth - progressBarWidth) / 2;
    int progressBarY = 20;

    float progressPercentage = m_Game.GetProgressBarPercentage();

    // Background
    DrawRectangle(progressBarX, progressBarY, progressBarWidth, progressBarHeight, Color{40, 40, 40, 200});

    // Fill
    int fillWidth = static_cast<int>(progressBarWidth * (progressPercentage / 100.0f));
    DrawRectangle(progressBarX, progressBarY, fillWidth, progressBarHeight, Color{0, 200, 100, 255});

    // Border
    DrawRectangleLines(progressBarX, progressBarY, progressBarWidth, progressBarHeight, WHITE);

    // Percentage Text
    char progressText[32];
    snprintf(progressText, sizeof(progressText), "%.0f%%", progressPercentage);
    int textWidth = MeasureText(progressText, 20);
    DrawText(progressText, progressBarX + (progressBarWidth - textWidth) / 2, progressBarY + 5, 20, WHITE);
}
