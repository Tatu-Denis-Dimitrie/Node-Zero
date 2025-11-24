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

class GameplayScreen : public IObserver, public std::enable_shared_from_this<GameplayScreen> {
   public:
    GameplayScreen(IGame& game, std::function<void(GameScreen)> stateChangeCallback, Font font);

    void Update(float deltaTime);
    void Draw();

    // IObserver implementation
    void Update(const std::shared_ptr<IEvent>& event) override;

   private:
    IGame& m_Game;
    std::function<void(GameScreen)> m_StateChangeCallback;
    std::vector<PickupCollectEffect> m_PickupEffects;
    Font m_Font;

    static constexpr float DAMAGE_ZONE_SIZE = 150.0f;
    static constexpr float DAMAGE_PER_TICK = 40.0f;
    static constexpr float PICKUP_COLLECT_EFFECT_DURATION = 1.0f;
    static constexpr float PICKUP_SPAWN_ANIM_DURATION = 0.45f;
};
