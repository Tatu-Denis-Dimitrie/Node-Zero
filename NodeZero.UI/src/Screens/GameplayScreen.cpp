#include "Screens/GameplayScreen.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "Events/GameEvents.h"
#include "INode.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

GameplayScreen::GameplayScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback), m_Font(font), m_ShakeIntensity(0.0f), m_ShakeDuration(0.0f), m_ShakeTimer(0.0f), m_ShakeOffset{0.0f, 0.0f} {
}

void GameplayScreen::Update(const std::shared_ptr<IEvent>& event) {
    auto gameEvent = std::static_pointer_cast<GameEvent>(event);
    if (gameEvent->type == EventType::NodeDamaged) {
        TriggerShake(SHAKE_INTENSITY, SHAKE_DURATION);

        Position nodePos = gameEvent->position;
        Vector2 particlePos = {nodePos.x, nodePos.y};
        SpawnDamageParticles(particlePos, RED, PARTICLE_COUNT);
    }
}

void GameplayScreen::TriggerShake(float intensity, float duration) {
    m_ShakeIntensity = intensity;
    m_ShakeDuration = duration;
    m_ShakeTimer = 0.0f;
}

void GameplayScreen::UpdateShake(float deltaTime) {
    if (m_ShakeTimer < m_ShakeDuration) {
        m_ShakeTimer += deltaTime;

        float progress = m_ShakeTimer / m_ShakeDuration;
        float currentIntensity = m_ShakeIntensity * (1.0f - progress);

        float randomX = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * currentIntensity;
        float randomY = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * currentIntensity;

        m_ShakeOffset = Vector2{randomX, randomY};
    } else {
        m_ShakeOffset = Vector2{0.0f, 0.0f};
    }
}

void GameplayScreen::SpawnDamageParticles(Vector2 position, Color baseColor, int count) {
    float screenScale = GetScreenHeight() / 800.0f;
    for (int i = 0; i < count; ++i) {
        DamageParticle particle;
        particle.position = position;

        float angle = (static_cast<float>(rand()) / RAND_MAX) * 6.28318530718f;
        float speed = PARTICLE_SPEED_MIN + (static_cast<float>(rand()) / RAND_MAX) * (PARTICLE_SPEED_MAX - PARTICLE_SPEED_MIN);

        particle.velocity.x = cos(angle) * speed;
        particle.velocity.y = sin(angle) * speed;

        particle.lifetime = 0.0f;
        particle.maxLifetime = PARTICLE_LIFETIME;
        float baseSize = 3.0f * screenScale;
        particle.size = baseSize + (static_cast<float>(rand()) / RAND_MAX) * baseSize;

        unsigned char r = baseColor.r + (rand() % 40) - 20;
        unsigned char g = baseColor.g + (rand() % 40) - 20;
        unsigned char b = baseColor.b + (rand() % 40) - 20;
        particle.color = Color{r, g, b, 255};

        m_DamageParticles.push_back(particle);
    }
}

void GameplayScreen::UpdateParticles(float deltaTime) {
    for (auto& particle : m_DamageParticles) {
        particle.lifetime += deltaTime;
        particle.position.x += particle.velocity.x * deltaTime;
        particle.position.y += particle.velocity.y * deltaTime;

        particle.velocity.y += 200.0f * deltaTime;

        float lifeRatio = particle.lifetime / particle.maxLifetime;
        particle.color.a = static_cast<unsigned char>((1.0f - lifeRatio) * 255.0f);
    }

    m_DamageParticles.erase(
        std::remove_if(m_DamageParticles.begin(), m_DamageParticles.end(),
                       [](const DamageParticle& p) { return p.lifetime >= p.maxLifetime; }),
        m_DamageParticles.end());
}

void GameplayScreen::ClearEffects() {
    m_DamageParticles.clear();
    m_PickupEffects.clear();
    m_ShakeTimer = m_ShakeDuration;
    m_ShakeOffset = Vector2{0.0f, 0.0f};
}

void GameplayScreen::DrawReflections(const std::vector<INode*>& nodes, Vector2 mousePos, float damageZoneSize, float reflectionOffset) {
    // Draw node reflections
    for (const INode* node : nodes) {
        if (node->GetState() == NodeState::Active) {
            float x = node->GetPosition().x + reflectionOffset;
            float y = node->GetPosition().y + reflectionOffset;
            float size = node->GetSize();
            float hpPercentage = node->GetHP() / node->GetMaxHP();
            float rotation = node->GetRotation();

            Color reflectionColor = RED;
            if (node->GetShape() == NodeShape::Boss) reflectionColor = Color{200, 50, 200, 255};
            reflectionColor.a = 5;

            switch (node->GetShape()) {
                case NodeShape::Circle:
                    Renderer::DrawCircleNode(x, y, size, hpPercentage, reflectionColor, rotation);
                    break;
                case NodeShape::Square:
                    Renderer::DrawSquareNode(x, y, size, hpPercentage, reflectionColor, rotation);
                    break;
                case NodeShape::Hexagon:
                    Renderer::DrawHexagonNode(x, y, size, hpPercentage, reflectionColor, rotation);
                    break;
                case NodeShape::Boss:
                    Renderer::DrawSquareNode(x, y, size, hpPercentage, reflectionColor, rotation);
                    break;
                default:
                    Renderer::DrawCircleNode(x, y, size, hpPercentage, reflectionColor, rotation);
                    break;
            }
        }
    }

    // Draw damage zone reflection
    float damageRectX = mousePos.x - damageZoneSize / 2.0f;
    float damageRectY = mousePos.y - damageZoneSize / 2.0f;

    DrawRectangle(
        static_cast<int>(damageRectX + reflectionOffset),
        static_cast<int>(damageRectY + reflectionOffset),
        static_cast<int>(damageZoneSize),
        static_cast<int>(damageZoneSize),
        Color{0, 100, 255, 5});

    // Draw reflection corners
    float cornerLength = GetScreenHeight() * 0.02f;
    float cornerThickness = GetScreenHeight() * 0.003f;
    Color cornerColor = Color{0, 200, 255, 255};
    Color reflectionCornerColor = cornerColor;
    reflectionCornerColor.a = 5;

    float rLeft = damageRectX + reflectionOffset;
    float rRight = mousePos.x + damageZoneSize / 2 + reflectionOffset;
    float rTop = damageRectY + reflectionOffset;
    float rBottom = mousePos.y + damageZoneSize / 2 + reflectionOffset;

    DrawLineEx(Vector2{rLeft, rTop}, Vector2{rLeft + cornerLength, rTop}, cornerThickness, reflectionCornerColor);
    DrawLineEx(Vector2{rLeft, rTop}, Vector2{rLeft, rTop + cornerLength}, cornerThickness, reflectionCornerColor);
    DrawLineEx(Vector2{rRight, rTop}, Vector2{rRight - cornerLength, rTop}, cornerThickness, reflectionCornerColor);
    DrawLineEx(Vector2{rRight, rTop}, Vector2{rRight, rTop + cornerLength}, cornerThickness, reflectionCornerColor);
    DrawLineEx(Vector2{rLeft, rBottom}, Vector2{rLeft + cornerLength, rBottom}, cornerThickness, reflectionCornerColor);
    DrawLineEx(Vector2{rLeft, rBottom}, Vector2{rLeft, rBottom - cornerLength}, cornerThickness, reflectionCornerColor);
    DrawLineEx(Vector2{rRight, rBottom}, Vector2{rRight - cornerLength, rBottom}, cornerThickness, reflectionCornerColor);
    DrawLineEx(Vector2{rRight, rBottom}, Vector2{rRight, rBottom - cornerLength}, cornerThickness, reflectionCornerColor);
}

void GameplayScreen::DrawBloom(const std::vector<INode*>& nodes, Vector2 mousePos, float damageZoneSize) {
    // Draw bloom for nodes
    for (const INode* node : nodes) {
        if (node->GetState() == NodeState::Active) {
            float x = node->GetPosition().x;
            float y = node->GetPosition().y;
            float size = node->GetSize();

            Color glowColor = RED;
            if (node->GetShape() == NodeShape::Boss) glowColor = Color{200, 50, 200, 255};

            glowColor.a = 40;
            DrawCircleGradient(static_cast<int>(x), static_cast<int>(y), size * 2.0f, glowColor, Fade(glowColor, 0.0f));
        }
    }

    // Draw bloom for damage zone
    Color zoneBloomColor = Color{0, 100, 255, 40};
    DrawCircleGradient(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y), damageZoneSize * 0.8f, zoneBloomColor, Fade(zoneBloomColor, 0.0f));
}

void GameplayScreen::Update(float deltaTime) {
    Vector2 mousePos = InputHandler::GetMousePosition();
    m_Game.SetMousePosition(mousePos.x, mousePos.y);

    m_Game.Update(deltaTime);

    UpdateShake(deltaTime);
    UpdateParticles(deltaTime);

    if (m_Game.ShouldGameOver()) {
        m_Game.SaveProgress();
        m_StateChangeCallback(GameScreen::GameOver);
        return;
    }

    std::vector<PointPickup> collectedPickups = m_Game.GetCollectedPickupsThisFrame();
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

    if (m_Game.IsLevelCompleted()) {
        m_StateChangeCallback(GameScreen::LevelCompleted);
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        m_StateChangeCallback(GameScreen::Paused);
    }
}

void GameplayScreen::Draw() {
    rlPushMatrix();
    rlTranslatef(m_ShakeOffset.x, m_ShakeOffset.y, 0.0f);

    Vector2 mousePos = InputHandler::GetMousePosition();
    float damageZoneSize = m_Game.GetDamageZoneSize();
    float damageRectX = mousePos.x - damageZoneSize / 2.0f;
    float damageRectY = mousePos.y - damageZoneSize / 2.0f;

    const auto& nodes = m_Game.GetNodes();

    // Apply visual effects (reflections and bloom)
    float reflectionOffset = GetScreenHeight() * 0.02f;
    DrawReflections(nodes, mousePos, damageZoneSize, reflectionOffset);
    DrawBloom(nodes, mousePos, damageZoneSize);

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

    for (const DamageParticle& particle : m_DamageParticles) {
        DrawCircleV(particle.position, particle.size, particle.color);
    }

    DrawRectangle(
        static_cast<int>(damageRectX),
        static_cast<int>(damageRectY),
        static_cast<int>(damageZoneSize),
        static_cast<int>(damageZoneSize),
        Color{0, 100, 255, 80});

    float cornerLength = GetScreenHeight() * 0.02f;
    float cornerThickness = GetScreenHeight() * 0.003f;
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

    float centerSquareSize = GetScreenHeight() * 0.008f;
    DrawRectangle(
        static_cast<int>(mousePos.x - centerSquareSize / 2),
        static_cast<int>(mousePos.y - centerSquareSize / 2),
        static_cast<int>(centerSquareSize),
        static_cast<int>(centerSquareSize),
        WHITE);

    Renderer::DrawProgressBar(m_Game.GetProgressBarPercentage(), m_Game.GetCurrentLevel(), m_Font);

    int healthBarX = static_cast<int>(GetScreenWidth() * 0.01f);
    int healthBarY = static_cast<int>(GetScreenHeight() * 0.01f);
    int healthBarWidth = static_cast<int>(GetScreenWidth() * 0.2f);
    int healthBarHeight = static_cast<int>(GetScreenHeight() * 0.03f);
    Renderer::DrawHealthBar(m_Game.GetCurrentHealth(), m_Game.GetMaxHealth(), healthBarX, healthBarY, healthBarWidth, healthBarHeight, m_Font);

    int pointsX = static_cast<int>(GetScreenWidth() * 0.01f);
    int pointsY = healthBarY + healthBarHeight + static_cast<int>(GetScreenHeight() * 0.015f);
    int pointsFontSize = static_cast<int>(GetScreenHeight() * 0.025f);
    Renderer::DrawPoints(m_Game.GetPickupPoints(), pointsX, pointsY, pointsFontSize, WHITE, m_Font);

    int debugX = GetScreenWidth() - static_cast<int>(GetScreenWidth() * 0.02f);  // 2% margin from right edge
    int debugY = static_cast<int>(GetScreenHeight() * 0.01f);
    Renderer::DrawDebugInfo(debugX, debugY, m_Font);

    rlPopMatrix();
}
