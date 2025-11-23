#pragma once

struct GameConfig {
    // Screen settings
    static constexpr float DEFAULT_SCREEN_WIDTH = 1200.0f;
    static constexpr float DEFAULT_SCREEN_HEIGHT = 800.0f;

    // Node settings
    static constexpr float NODE_MIN_SIZE = 20.0f;
    static constexpr float NODE_MAX_SIZE = 60.0f;
    static constexpr float NODE_MIN_SPEED = 50.0f;
    static constexpr float NODE_MAX_SPEED = 200.0f;
    static constexpr float NODE_DEFAULT_SIZE = 30.0f;
    static constexpr float NODE_DEFAULT_SPEED = 100.0f;
    static constexpr int MAX_NODES_PER_LEVEL = 50;
    static constexpr float NODE_SPAWN_INTERVAL = 2.0f;

    // Pickup settings
    static constexpr float PICKUP_LIFETIME = 10.0f;
    static constexpr float PICKUP_SIZE = 6.0f;
    static constexpr float PICKUP_COLLECT_DELAY = 0.1f;

    // Score settings
    static constexpr int POINTS_PER_NODE_DESTROYED = 100;
    static constexpr int POINTS_MULTIPLIER_MAX = 5;

    // Health settings
    static constexpr int HEALTH_UPGRADE_COST = 50;
    static constexpr int REGEN_UPGRADE_COST = 100;
    static constexpr float REGEN_UPGRADE_AMOUNT = 0.1f;

    // Damage settings
    static constexpr int DAMAGE_ZONE_UPGRADE_COST = 75;
    static constexpr float DAMAGE_ZONE_UPGRADE_AMOUNT = 10.0f;
    static constexpr int DAMAGE_UPGRADE_COST = 60;
    static constexpr float DAMAGE_UPGRADE_AMOUNT = 5.0f;

    // Boss and level settings
    static constexpr int NODES_PER_LEVEL = 20;
    static constexpr float BOSS_SIZE = 120.0f;
    static constexpr float BOSS_SPEED = 35.0f;
    static constexpr float BOSS_HP_BASE = 200.0f;
    static constexpr float LEVEL_DURATION = 60.0f;  // 60 seconds per level
};
