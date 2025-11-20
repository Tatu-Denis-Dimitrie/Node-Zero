#pragma once
#include <iostream>
#include <memory>

#include "Events/GameEvents.h"
#include "Events/IObserver.h"

class GameEventLogger : public IObserver {
   public:
    GameEventLogger() = default;
    virtual ~GameEventLogger() = default;

    void OnEvent(const std::shared_ptr<IEvent>& event) override {
        if (!event) return;

        const std::string& eventType = event->GetType();

        if (eventType == "NodeSpawned") {
            HandleNodeSpawned(std::static_pointer_cast<NodeSpawnedEvent>(event));
        } else if (eventType == "NodeDestroyed") {
            HandleNodeDestroyed(std::static_pointer_cast<NodeDestroyedEvent>(event));
        } else if (eventType == "NodeDamaged") {
            HandleNodeDamaged(std::static_pointer_cast<NodeDamagedEvent>(event));
        } else if (eventType == "ScoreChanged") {
            HandleScoreChanged(std::static_pointer_cast<ScoreChangedEvent>(event));
        } else if (eventType == "GameStateChanged") {
            HandleGameStateChanged(std::static_pointer_cast<GameStateChangedEvent>(event));
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
                  << ") | Score gained: " << event->GetScoreGained()
                  << std::endl;
    }

    void HandleNodeDamaged(const std::shared_ptr<NodeDamagedEvent>& event) {
        std::cout << "[EVENT] Node damaged | Damage: " << event->GetDamage()
                  << " | Remaining HP: " << event->GetRemainingHP()
                  << std::endl;
    }

    void HandleScoreChanged(const std::shared_ptr<ScoreChangedEvent>& event) {
        std::cout << "[EVENT] Score changed | New Score: " << event->GetNewScore()
                  << " | Delta: " << event->GetDelta()
                  << " | Multiplier: " << event->GetMultiplier()
                  << std::endl;
    }

    void HandleGameStateChanged(const std::shared_ptr<GameStateChangedEvent>& event) {
        std::cout << "[EVENT] Game state changed | New State: "
                  << static_cast<int>(event->GetNewState())
                  << " | Old State: " << static_cast<int>(event->GetOldState())
                  << std::endl;
    }
};
