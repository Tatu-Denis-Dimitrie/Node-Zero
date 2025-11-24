#include "../../include/Screens/GameplayScreen.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "../../include/InputHandler.h"
#include "../../include/Renderer.h"
#include "Events/GameEvents.h"
#include "INode.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

GameplayScreen::GameplayScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font)
    : m_Game(game), m_StateChangeCallback(stateChangeCallback), m_Font(font), m_ShakeIntensity(0.0f), m_ShakeDuration(0.0f), m_ShakeTimer(0.0f), m_ShakeOffset{0.0f, 0.0f} {
}

void GameplayScreen::Update(const std::shared_ptr<IEvent>& event) {
    // React to game events through Observer pattern
    if (event->GetType() == "NodeDamaged") {
        // Trigger camera shake when nodes take damage
        TriggerShake(SHAKE_INTENSITY, SHAKE_DURATION);

        // Spawn damage particles
        auto damageEvent = std::static_pointer_cast<NodeDamagedEvent>(event);
        Position nodePos = damageEvent->GetPosition();
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

        // Calculate shake intensity that decays over time
        float progress = m_ShakeTimer / m_ShakeDuration;
        float currentIntensity = m_ShakeIntensity * (1.0f - progress);

        // Generate random shake offset
        float randomX = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * currentIntensity;
        float randomY = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * currentIntensity;

        m_ShakeOffset = Vector2{randomX, randomY};
    } else {
        m_ShakeOffset = Vector2{0.0f, 0.0f};
    }
}

void GameplayScreen::SpawnDamageParticles(Vector2 position, Color baseColor, int count) {
    for (int i = 0; i < count; ++i) {
        DamageParticle particle;
        particle.position = position;

        // Random angle for particle direction
        float angle = (static_cast<float>(rand()) / RAND_MAX) * 6.28318530718f;  // 2 * PI
        float speed = PARTICLE_SPEED_MIN + (static_cast<float>(rand()) / RAND_MAX) * (PARTICLE_SPEED_MAX - PARTICLE_SPEED_MIN);

        particle.velocity.x = cos(angle) * speed;
        particle.velocity.y = sin(angle) * speed;

        particle.lifetime = 0.0f;
        particle.maxLifetime = PARTICLE_LIFETIME;
        particle.size = 3.0f + (static_cast<float>(rand()) / RAND_MAX) * 3.0f;  // Random size between 3-6

        // Vary the color slightly
        unsigned char r = baseColor.r + (rand() % 40) - 20;
        unsigned char g = baseColor.g + (rand() % 40) - 20;
        unsigned char b = baseColor.b + (rand() % 40) - 20;
        particle.color = Color{r, g, b, 255};

        m_DamageParticles.push_back(particle);
    }
}

void GameplayScreen::UpdateParticles(float deltaTime) {
    // Update all particles
    for (auto& particle : m_DamageParticles) {
        particle.lifetime += deltaTime;
        particle.position.x += particle.velocity.x * deltaTime;
        particle.position.y += particle.velocity.y * deltaTime;

        // Apply gravity
        particle.velocity.y += 200.0f * deltaTime;

        // Fade out based on lifetime
        float lifeRatio = particle.lifetime / particle.maxLifetime;
        particle.color.a = static_cast<unsigned char>((1.0f - lifeRatio) * 255.0f);
    }

    // Remove dead particles
    m_DamageParticles.erase(
        std::remove_if(m_DamageParticles.begin(), m_DamageParticles.end(),
                       [](const DamageParticle& p) { return p.lifetime >= p.maxLifetime; }),
        m_DamageParticles.end());
}

void GameplayScreen::Update(float deltaTime) {
    Vector2 mousePos = InputHandler::GetMousePosition();
    m_Game.SetMousePosition(mousePos.x, mousePos.y);

    m_Game.Update(deltaTime);

    // Update camera shake
    UpdateShake(deltaTime);

    // Update damage particles
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
    // Apply camera shake offset
    rlPushMatrix();
    rlTranslatef(m_ShakeOffset.x, m_ShakeOffset.y, 0.0f);

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

    // Draw damage particles
    for (const DamageParticle& particle : m_DamageParticles) {
        DrawCircleV(particle.position, particle.size, particle.color);
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

    Renderer::DrawProgressBar(m_Game.GetProgressBarPercentage(), m_Game.GetCurrentLevel(), m_Font);

    // Draw HUD elements (affected by camera shake)
    Renderer::DrawHealthBar(m_Game.GetCurrentHealth(), m_Game.GetMaxHealth(), 10, 10, 300, 24, m_Font);
    Renderer::DrawScore(m_Game.GetPickupScore(), 10, 45, 20, WHITE, m_Font);
    Renderer::DrawDebugInfo(GetScreenWidth() - 100, 10, m_Font);

    // Restore original matrix (remove shake offset)
    rlPopMatrix();
}
