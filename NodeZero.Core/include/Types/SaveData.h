#pragma once

#include "../Config/GameConfig.h"

struct SaveData {
    int highPoints = 0;
    int points = 0;
    int gamesPlayed = 0;
    int totalNodesDestroyed = 0;
    int currentLevel = 1;
    float maxHealth = GameConfig::HEALTH_DEFAULT;
    float regenRate = 0.0f;
    float damageZoneSize = GameConfig::DAMAGE_ZONE_DEFAULT_SIZE;
    float damagePerTick = GameConfig::DAMAGE_PER_TICK_DEFAULT;
};
