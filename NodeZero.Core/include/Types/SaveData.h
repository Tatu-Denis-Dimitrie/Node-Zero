#pragma once

struct SaveData {
    int highScore = 0;
    int coins = 0;
    int gamesPlayed = 0;
    int totalNodesDestroyed = 0;
    float maxHealth = 15.0f;
    float regenRate = 0.0f;
    float damageZoneSize = 50.0f;
    float damagePerTick = 40.0f;
};
