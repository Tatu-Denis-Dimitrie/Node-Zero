#pragma once
#include "IEvent.h"
#include "Enums/NodeState.h"
#include "Enums/GameState.h"
#include "Types/Vector2.h"

namespace NodeZero {

/**
 * @brief Base class for all game events with timestamp support
 */
class GameEvent : public IEvent {
public:
    explicit GameEvent(float timestamp) : m_timestamp(timestamp) {}
    virtual ~GameEvent() = default;

    float GetTimestamp() const override { return m_timestamp; }

private:
    float m_timestamp;
};

// ============================================================================
// Node Events
// ============================================================================

/**
 * @brief Event fired when a new node is spawned
 */
class NodeSpawnedEvent : public GameEvent {
public:
    NodeSpawnedEvent(float timestamp, NodeShape shape, Vector2 position, float size, int hp)
        : GameEvent(timestamp), m_shape(shape), m_position(position), m_size(size), m_hp(hp) {}

    std::string GetType() const override { return "NodeSpawned"; }

    NodeShape GetShape() const { return m_shape; }
    Vector2 GetPosition() const { return m_position; }
    float GetSize() const { return m_size; }
    int GetHP() const { return m_hp; }

private:
    NodeShape m_shape;
    Vector2 m_position;
    float m_size;
    int m_hp;
};

/**
 * @brief Event fired when a node takes damage
 */
class NodeDamagedEvent : public GameEvent {
public:
    NodeDamagedEvent(float timestamp, Vector2 position, int damage, int remainingHP)
        : GameEvent(timestamp), m_position(position), m_damage(damage), m_remainingHP(remainingHP) {}

    std::string GetType() const override { return "NodeDamaged"; }

    Vector2 GetPosition() const { return m_position; }
    int GetDamage() const { return m_damage; }
    int GetRemainingHP() const { return m_remainingHP; }

private:
    Vector2 m_position;
    int m_damage;
    int m_remainingHP;
};

/**
 * @brief Event fired when a node is destroyed
 */
class NodeDestroyedEvent : public GameEvent {
public:
    NodeDestroyedEvent(float timestamp, NodeShape shape, Vector2 position, int scoreGained)
        : GameEvent(timestamp), m_shape(shape), m_position(position), m_scoreGained(scoreGained) {}

    std::string GetType() const override { return "NodeDestroyed"; }

    NodeShape GetShape() const { return m_shape; }
    Vector2 GetPosition() const { return m_position; }
    int GetScoreGained() const { return m_scoreGained; }

private:
    NodeShape m_shape;
    Vector2 m_position;
    int m_scoreGained;
};

// ============================================================================
// Score Events
// ============================================================================

/**
 * @brief Event fired when the score changes
 */
class ScoreChangedEvent : public GameEvent {
public:
    ScoreChangedEvent(float timestamp, int newScore, int delta, float multiplier)
        : GameEvent(timestamp), m_newScore(newScore), m_delta(delta), m_multiplier(multiplier) {}

    std::string GetType() const override { return "ScoreChanged"; }

    int GetNewScore() const { return m_newScore; }
    int GetDelta() const { return m_delta; }
    float GetMultiplier() const { return m_multiplier; }

private:
    int m_newScore;
    int m_delta;
    float m_multiplier;
};

/**
 * @brief Event fired when the score multiplier changes
 */
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

// ============================================================================
// Game State Events
// ============================================================================

/**
 * @brief Event fired when the game state changes
 */
class GameStateChangedEvent : public GameEvent {
public:
    GameStateChangedEvent(float timestamp, GameState newState, GameState oldState)
        : GameEvent(timestamp), m_newState(newState), m_oldState(oldState) {}

    std::string GetType() const override { return "GameStateChanged"; }

    GameState GetNewState() const { return m_newState; }
    GameState GetOldState() const { return m_oldState; }

private:
    GameState m_newState;
    GameState m_oldState;
};

/**
 * @brief Event fired when the game is over
 */
class GameOverEvent : public GameEvent {
public:
    GameOverEvent(float timestamp, int finalScore)
        : GameEvent(timestamp), m_finalScore(finalScore) {}

    std::string GetType() const override { return "GameOver"; }

    int GetFinalScore() const { return m_finalScore; }

private:
    int m_finalScore;
};

} // namespace NodeZero
