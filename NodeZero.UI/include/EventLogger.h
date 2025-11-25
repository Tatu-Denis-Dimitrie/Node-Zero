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

        const std::string& eventType = event->GetType();

        if (eventType == "NodeSpawned") {
            HandleNodeSpawned(std::static_pointer_cast<NodeSpawnedEvent>(event));
        } else if (eventType == "NodeDestroyed") {
            HandleNodeDestroyed(std::static_pointer_cast<NodeDestroyedEvent>(event));
        } else if (eventType == "NodeDamaged") {
            HandleNodeDamaged(std::static_pointer_cast<NodeDamagedEvent>(event));
        } else if (eventType == "BossSpawned") {
            HandleBossSpawned(std::static_pointer_cast<BossSpawnedEvent>(event));
        } else if (eventType == "BossDefeated") {
            HandleBossDefeated(std::static_pointer_cast<BossDefeatedEvent>(event));
        } else if (eventType == "LevelCompleted") {
            HandleLevelCompleted(std::static_pointer_cast<LevelCompletedEvent>(event));
        }
    }

   private:
    void HandleNodeSpawned(const std::shared_ptr<NodeSpawnedEvent>& event) {
        std::cout << "[EVENT] Node spawned at ("
                  << event->GetPosition().x << ", "
                  << event->GetPosition().y
                  << ") | HP: " << event->GetHP()
                  << " | Size: " << event->GetSize()
                  << std::endl;
    }

    void HandleNodeDestroyed(const std::shared_ptr<NodeDestroyedEvent>& event) {
        std::cout << "[EVENT] Node destroyed at ("
                  << event->GetPosition().x << ", "
                  << event->GetPosition().y
                  << ") | Points gained: " << event->GetPointsGained()
                  << std::endl;
    }

    void HandleNodeDamaged(const std::shared_ptr<NodeDamagedEvent>& event) {
        std::cout << "[EVENT] Node damaged | Damage: " << event->GetDamage()
                  << " | Remaining HP: " << event->GetRemainingHP()
                  << std::endl;
    }

    void HandleBossSpawned(const std::shared_ptr<BossSpawnedEvent>& event) {
        std::cout << "[EVENT] Boss spawned | Level: " << event->GetLevel()
                  << " | HP: " << event->GetBossHP()
                  << std::endl;
    }

    void HandleBossDefeated(const std::shared_ptr<BossDefeatedEvent>& event) {
        std::cout << "[EVENT] Boss defeated | Level: " << event->GetLevel()
                  << " | Points gained: " << event->GetPointsGained()
                  << std::endl;
    }

    void HandleLevelCompleted(const std::shared_ptr<LevelCompletedEvent>& event) {
        std::cout << "[EVENT] Level completed | Old Level: " << event->GetOldLevel()
                  << " | New Level: " << event->GetNewLevel()
                  << std::endl;
    }
};
