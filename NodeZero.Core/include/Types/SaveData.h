#pragma once

struct SaveData {
    int highPoints = 0;
    int points = 0;
    int gamesPlayed = 0;
    int totalNodesDestroyed = 0;
    int currentLevel = 1;
    float maxHealth = 15.0f;
    float regenRate = 0.0f;
    float damageZoneSize = 50.0f;
    float damagePerTick = 40.0f;
};
