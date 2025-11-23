#pragma once

#include <functional>
#include <vector>

#include "Enums/GameScreen.h"
#include "IGame.h"
#include "raylib.h"

struct PickupCollectEffect {
    Vector2 startPosition;
    float elapsed;
    float duration;
    float size;
};

class GameplayScreen {
   public:
    GameplayScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback);

    void Update(float deltaTime);
    void Draw();

   private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    std::vector<PickupCollectEffect> m_PickupEffects;

    static constexpr float DAMAGE_ZONE_SIZE = 150.0f;
    static constexpr float DAMAGE_PER_TICK = 40.0f;
    // PICKUP_COLLECT_DELAY movută în Game.h (evită duplicare)
    static constexpr float PICKUP_COLLECT_EFFECT_DURATION = 1.0f;
    static constexpr float PICKUP_SPAWN_ANIM_DURATION = 0.45f;
};
