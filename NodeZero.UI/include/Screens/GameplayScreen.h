#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Enums/GameScreen.h"
#include "Events/IObserver.h"
#include "IGame.h"
#include "raylib.h"

struct PickupCollectEffect {
    Vector2 startPosition;
    float elapsed;
    float duration;
    float size;
};

struct DamageParticle {
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    float maxLifetime;
    float size;
    Color color;
};

class GameplayScreen : public IObserver, public std::enable_shared_from_this<GameplayScreen> {
   public:
    GameplayScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

    void Update(const std::shared_ptr<IEvent>& event) override;

   private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    std::vector<PickupCollectEffect> m_PickupEffects;
    std::vector<DamageParticle> m_DamageParticles;
    Font m_Font;

    float m_ShakeIntensity;
    float m_ShakeDuration;
    float m_ShakeTimer;
    Vector2 m_ShakeOffset;

    void TriggerShake(float intensity, float duration);
    void UpdateShake(float deltaTime);
    void SpawnDamageParticles(Vector2 position, Color baseColor, int count);
    void UpdateParticles(float deltaTime);

    static constexpr float DAMAGE_ZONE_SIZE = 150.0f;
    static constexpr float DAMAGE_PER_TICK = 40.0f;
    static constexpr float PICKUP_COLLECT_EFFECT_DURATION = 1.0f;
    static constexpr float PICKUP_SPAWN_ANIM_DURATION = 0.45f;
    static constexpr float SHAKE_INTENSITY = 8.0f;
    static constexpr float SHAKE_DURATION = 0.15f;
    static constexpr float PARTICLE_LIFETIME = 0.6f;
    static constexpr float PARTICLE_SPEED_MIN = 50.0f;
    static constexpr float PARTICLE_SPEED_MAX = 150.0f;
    static constexpr int PARTICLE_COUNT = 8;
};
