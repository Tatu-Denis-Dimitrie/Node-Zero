#pragma once

enum class EventType {
    NodeSpawned,
    NodeDamaged,
    NodeDestroyed,
    PointsChanged,
    MultiplierChanged,
    GameStateChanged,
    GameOver,
    BossSpawned,
    BossDefeated,
    LevelCompleted
};
