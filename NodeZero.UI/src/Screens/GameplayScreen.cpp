#include "../../include/Screens/GameplayScreen.h"

#include <algorithm>

#include "../../include/InputHandler.h"
#include "../../include/Renderer.h"
#include "INode.h"
#include "raymath.h"

GameplayScreen::GameplayScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback), m_Font(font) {
}

void GameplayScreen::Update(float deltaTime) {
    Vector2 mousePos = InputHandler::GetMousePosition();

    m_Game.UpdateHealth(deltaTime);
    m_Game.UpdateAutoSpawn(deltaTime);
    m_Game.UpdateDamageTimer(deltaTime);

    bool shouldDealDamage = m_Game.ShouldDealDamage();
    if (shouldDealDamage) {
        m_Game.ResetDamageTimer();
    }
    float damageZoneSize = m_Game.GetDamageZoneSize();
    float damagePerTick = m_Game.GetDamagePerTick();
    m_Game.ProcessDamageZone(mousePos.x, mousePos.y, damageZoneSize, damagePerTick, shouldDealDamage);

    // Verificare game over o singură dată (după toate update-urile)
    if (m_Game.ShouldGameOver()) {
        m_Game.SaveProgress();
        m_Game.Reset();
        m_StateChangeCallback(GameScreen::MainMenu);
        return;
    }

    // Colectare pickups - Core returnează pickup-urile colectate (fără logică de coliziune în UI)
    std::vector<PointPickup> collectedPickups = m_Game.ProcessPickupCollection(mousePos.x, mousePos.y, damageZoneSize);

    // Creăm efecte vizuale pentru pickup-urile colectate
    for (const PointPickup& pickup : collectedPickups) {
        PickupCollectEffect effect{};
        effect.startPosition = Vector2{pickup.position.x, pickup.position.y};
        effect.elapsed = 0.0f;
        effect.duration = PICKUP_COLLECT_EFFECT_DURATION;
        effect.size = pickup.size;
        m_PickupEffects.push_back(effect);
    }

    for (auto& effect : m_PickupEffects) {
        effect.elapsed += deltaTime;
    }

    m_PickupEffects.erase(
        std::remove_if(m_PickupEffects.begin(), m_PickupEffects.end(),
                       [](const PickupCollectEffect& effect) { return effect.elapsed >= effect.duration; }),
        m_PickupEffects.end());

    m_Game.Update(deltaTime);

    if (m_Game.IsLevelCompleted()) {
        m_StateChangeCallback(GameScreen::LevelCompleted);
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameScreen::Paused);
    }
}

void GameplayScreen::Draw() {
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
                    Renderer::DrawCircleNode(x, y, size, hpPercentage, RED, rotation);
                    break;
                case NodeShape::Square:
                    Renderer::DrawSquareNode(x, y, size, hpPercentage, RED, rotation);
                    break;
                case NodeShape::Hexagon:
                    Renderer::DrawHexagonNode(x, y, size, hpPercentage, RED, rotation);
                    break;
                case NodeShape::Boss:
                    // Boss: Pink-purple large square
                    Renderer::DrawSquareNode(x, y, size, hpPercentage, Color{200, 50, 200, 255}, rotation);
                    break;
                default:
                    Renderer::DrawCircleNode(x, y, size, hpPercentage, RED, rotation);
                    break;
            }
        }
    }

    const auto& pickups = m_Game.GetPickups();
    for (const PointPickup& pickup : pickups) {
        float lifeRatio = std::clamp(pickup.GetLifeRatio(), 0.0f, 1.0f);
        unsigned char alpha = static_cast<unsigned char>(lifeRatio * 255.0f);
        Color pickupColor = Color{255, 50, 50, alpha};
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
            pickupColor = Color{255, 80, 80, alpha};
        }
        Renderer::DrawPickup(pickupPos.x, pickupPos.y, pickupSize, pickupColor);
    }

    for (const PickupCollectEffect& effect : m_PickupEffects) {
        float t = std::clamp(effect.elapsed / effect.duration, 0.0f, 1.0f);
        Vector2 targetPos{mousePos.x, mousePos.y};
        Vector2 currentPos = Vector2Lerp(effect.startPosition, targetPos, t);
        unsigned char alpha = static_cast<unsigned char>((1.0f - t) * 255.0f);
        Renderer::DrawPickup(currentPos.x, currentPos.y, effect.size, Color{255, 50, 50, alpha});
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
    Renderer::DrawProgressBar(m_Game.GetProgressBarPercentage(), m_Game.GetCurrentLevel(), m_Font);
}
