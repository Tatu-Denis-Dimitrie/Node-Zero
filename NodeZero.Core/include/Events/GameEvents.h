#pragma once
#include <string>

#include "Enums/EventType.h"
#include "Enums/GameScreen.h"
#include "Enums/NodeShape.h"
#include "Enums/NodeState.h"
#include "IEvent.h"
#include "Types/Position.h"

class GameEvent : public IEvent {
   public:
    EventType type;
    float timestamp;

    NodeShape shape;
    Position position;
    float size;
    int hp;
    int damage;
    int points;
    int delta;
    float multiplier;
    float oldMultiplier;
    GameScreen screenState;
    GameScreen oldScreenState;
    int level;
    int nextLevel;
    float bossHP;

    GameEvent(float timestamp, EventType type) : timestamp(timestamp), type(type) {}

    float GetTimestamp() const override { return timestamp; }

    std::string GetType() const override {
        switch (type) {
            case EventType::NodeSpawned:
                return "NodeSpawned";
            case EventType::NodeDamaged:
                return "NodeDamaged";
            case EventType::NodeDestroyed:
                return "NodeDestroyed";
            case EventType::PointsChanged:
                return "PointsChanged";
            case EventType::MultiplierChanged:
                return "MultiplierChanged";
            case EventType::GameStateChanged:
                return "GameStateChanged";
            case EventType::GameOver:
                return "GameOver";
            case EventType::BossSpawned:
                return "BossSpawned";
            case EventType::BossDefeated:
                return "BossDefeated";
            case EventType::LevelCompleted:
                return "LevelCompleted";
            default:
                return "Unknown";
        }
    }
};
