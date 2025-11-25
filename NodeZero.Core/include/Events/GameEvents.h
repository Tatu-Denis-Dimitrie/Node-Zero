#pragma once
#include "Enums/GameScreen.h"
#include "Enums/NodeShape.h"
#include "Enums/NodeState.h"
#include "IEvent.h"
#include "Types/Position.h"

class GameEvent : public IEvent {
   public:
    explicit GameEvent(float timestamp) : m_timestamp(timestamp) {}
    virtual ~GameEvent() = default;

    float GetTimestamp() const override { return m_timestamp; }

   private:
    float m_timestamp;
};

class NodeSpawnedEvent : public GameEvent {
   public:
    NodeSpawnedEvent(float timestamp, NodeShape shape, Position position, float size, int hp)
        : GameEvent(timestamp), m_shape(shape), m_position(position), m_size(size), m_hp(hp) {}

    std::string GetType() const override { return "NodeSpawned"; }

    NodeShape GetShape() const { return m_shape; }
    Position GetPosition() const { return m_position; }
    float GetSize() const { return m_size; }
    int GetHP() const { return m_hp; }

   private:
    NodeShape m_shape;
    Position m_position;
    float m_size;
    int m_hp;
};

class NodeDamagedEvent : public GameEvent {
   public:
    NodeDamagedEvent(float timestamp, Position position, int damage, int remainingHP)
        : GameEvent(timestamp), m_position(position), m_damage(damage), m_remainingHP(remainingHP) {}

    std::string GetType() const override { return "NodeDamaged"; }

    Position GetPosition() const { return m_position; }
    int GetDamage() const { return m_damage; }
    int GetRemainingHP() const { return m_remainingHP; }

   private:
    Position m_position;
    int m_damage;
    int m_remainingHP;
};

class NodeDestroyedEvent : public GameEvent {
   public:
    NodeDestroyedEvent(float timestamp, NodeShape shape, Position position, int pointsGained)
        : GameEvent(timestamp), m_shape(shape), m_position(position), m_pointsGained(pointsGained) {}

    std::string GetType() const override { return "NodeDestroyed"; }

    NodeShape GetShape() const { return m_shape; }
    Position GetPosition() const { return m_position; }
    int GetPointsGained() const { return m_pointsGained; }

   private:
    NodeShape m_shape;
    Position m_position;
    int m_pointsGained;
};

class PointsChangedEvent : public GameEvent {
   public:
    PointsChangedEvent(float timestamp, int newPoints, int delta, float multiplier)
        : GameEvent(timestamp), m_newPoints(newPoints), m_delta(delta), m_multiplier(multiplier) {}

    std::string GetType() const override { return "PointsChanged"; }

    int GetNewPoints() const { return m_newPoints; }
    int GetDelta() const { return m_delta; }
    float GetMultiplier() const { return m_multiplier; }

   private:
    int m_newPoints;
    int m_delta;
    float m_multiplier;
};

class MultiplierChangedEvent : public GameEvent {
   public:
    MultiplierChangedEvent(float timestamp, float newMultiplier, float oldMultiplier)
        : GameEvent(timestamp), m_newMultiplier(newMultiplier), m_oldMultiplier(oldMultiplier) {}

    std::string GetType() const override { return "MultiplierChanged"; }

    float GetNewMultiplier() const { return m_newMultiplier; }
    float GetOldMultiplier() const { return m_oldMultiplier; }

   private:
    float m_newMultiplier;
    float m_oldMultiplier;
};

class GameStateChangedEvent : public GameEvent {
   public:
    GameStateChangedEvent(float timestamp, GameScreen newState, GameScreen oldState)
        : GameEvent(timestamp), m_newState(newState), m_oldState(oldState) {}

    std::string GetType() const override { return "GameStateChanged"; }

    GameScreen GetNewState() const { return m_newState; }
    GameScreen GetOldState() const { return m_oldState; }

   private:
    GameScreen m_newState;
    GameScreen m_oldState;
};

class GameOverEvent : public GameEvent {
   public:
    GameOverEvent(float timestamp, int finalPoints)
        : GameEvent(timestamp), m_finalPoints(finalPoints) {}

    std::string GetType() const override { return "GameOver"; }

    int GetFinalPoints() const { return m_finalPoints; }

   private:
    int m_finalPoints;
};

class BossSpawnedEvent : public GameEvent {
   public:
    BossSpawnedEvent(float timestamp, int level, float bossHP)
        : GameEvent(timestamp), m_level(level), m_bossHP(bossHP) {}

    std::string GetType() const override { return "BossSpawned"; }

    int GetLevel() const { return m_level; }
    float GetBossHP() const { return m_bossHP; }

   private:
    int m_level;
    float m_bossHP;
};

class BossDefeatedEvent : public GameEvent {
   public:
    BossDefeatedEvent(float timestamp, int level, int pointsGained)
        : GameEvent(timestamp), m_level(level), m_pointsGained(pointsGained) {}

    std::string GetType() const override { return "BossDefeated"; }

    int GetLevel() const { return m_level; }
    int GetPointsGained() const { return m_pointsGained; }

   private:
    int m_level;
    int m_pointsGained;
};

class LevelCompletedEvent : public GameEvent {
   public:
    LevelCompletedEvent(float timestamp, int oldLevel, int newLevel)
        : GameEvent(timestamp), m_oldLevel(oldLevel), m_newLevel(newLevel) {}

    std::string GetType() const override { return "LevelCompleted"; }

    int GetOldLevel() const { return m_oldLevel; }
    int GetNewLevel() const { return m_newLevel; }

   private:
    int m_oldLevel;
    int m_newLevel;
};
