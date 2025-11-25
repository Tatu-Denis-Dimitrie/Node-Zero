#pragma once

struct GameConfig {
    // Node settings
    static constexpr float NODE_DEFAULT_SPEED = 75.0f;

    // Pickup settings
    static constexpr float PICKUP_LIFETIME = 10.0f;
    static constexpr float PICKUP_COLLECT_DELAY = 0.1f;

    // Points settings
    static constexpr int POINTS_MULTIPLIER_MAX = 5;

    // Health settings
    static constexpr float HEALTH_DEFAULT = 10.0f;
    static constexpr int HEALTH_UPGRADE_COST = 50;
    static constexpr int REGEN_UPGRADE_COST = 100;
    static constexpr float REGEN_UPGRADE_AMOUNT = 0.1f;

    // Damage settings
    static constexpr float DAMAGE_ZONE_DEFAULT_SIZE = 70.0f;
    static constexpr int DAMAGE_ZONE_UPGRADE_COST = 75;
    static constexpr float DAMAGE_ZONE_UPGRADE_AMOUNT = 10.0f;
    static constexpr float DAMAGE_ZONE_MAX_SIZE = 300.0f;
    static constexpr float DAMAGE_PER_TICK_DEFAULT = 40.0f;
    static constexpr int DAMAGE_UPGRADE_COST = 60;
    static constexpr float DAMAGE_UPGRADE_AMOUNT = 5.0f;

    // Boss and level settings
    static constexpr float BOSS_SPEED = 35.0f;
    static constexpr float BOSS_HP_BASE = 200.0f;
    static constexpr float LEVEL_DURATION = 60.0f;
};
