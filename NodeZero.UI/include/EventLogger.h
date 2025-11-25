#pragma once
#include <iostream>
#include <memory>

#include "Events/GameEvents.h"
#include "Events/IObserver.h"

class EventLogger : public IObserver {
   public:
    EventLogger() = default;
    virtual ~EventLogger() = default;

    void Update(const std::shared_ptr<IEvent>& event) override {
        if (!event) return;

        auto gameEvent = std::static_pointer_cast<GameEvent>(event);

        switch (gameEvent->type) {
            case EventType::NodeSpawned:
                HandleNodeSpawned(gameEvent);
                break;
            case EventType::NodeDestroyed:
                HandleNodeDestroyed(gameEvent);
                break;
            case EventType::NodeDamaged:
                HandleNodeDamaged(gameEvent);
                break;
            case EventType::BossSpawned:
                HandleBossSpawned(gameEvent);
                break;
            case EventType::BossDefeated:
                HandleBossDefeated(gameEvent);
                break;
            case EventType::LevelCompleted:
                HandleLevelCompleted(gameEvent);
                break;
            default:
                break;
        }
    }

   private:
    void HandleNodeSpawned(const std::shared_ptr<GameEvent>& event) {
        std::cout << "[EVENT] Node spawned at ("
                  << event->position.x << ", "
                  << event->position.y
                  << ") | HP: " << event->hp
                  << " | Size: " << event->size
                  << std::endl;
    }

    void HandleNodeDestroyed(const std::shared_ptr<GameEvent>& event) {
        std::cout << "[EVENT] Node destroyed at ("
                  << event->position.x << ", "
                  << event->position.y
                  << ") | Points gained: " << event->points
                  << std::endl;
    }

    void HandleNodeDamaged(const std::shared_ptr<GameEvent>& event) {
        std::cout << "[EVENT] Node damaged | Damage: " << event->damage
                  << " | Remaining HP: " << event->hp
                  << std::endl;
    }

    void HandleBossSpawned(const std::shared_ptr<GameEvent>& event) {
        std::cout << "[EVENT] Boss spawned | Level: " << event->level
                  << " | HP: " << event->bossHP
                  << std::endl;
    }

    void HandleBossDefeated(const std::shared_ptr<GameEvent>& event) {
        std::cout << "[EVENT] Boss defeated | Level: " << event->level
                  << " | Points gained: " << event->points
                  << std::endl;
    }

    void HandleLevelCompleted(const std::shared_ptr<GameEvent>& event) {
        std::cout << "[EVENT] Level completed | Old Level: " << event->level
                  << " | New Level: " << event->nextLevel
                  << std::endl;
    }
};
