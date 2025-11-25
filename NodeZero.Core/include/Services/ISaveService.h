#pragma once

#include "Types/SaveData.h"

class ISaveService {
   public:
    virtual ~ISaveService() = default;

    virtual SaveData LoadProgress() = 0;
    virtual void SaveProgress(const SaveData& data) = 0;

    virtual void UpdatePoints(int points) = 0;
    virtual void UpdateHighPoints(int highPoints) = 0;
    virtual void UpdateGamesPlayed(int gamesPlayed) = 0;
    virtual void UpdateTotalNodesDestroyed(int totalNodesDestroyed) = 0;
    virtual void UpdateStats(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick, int currentLevel) = 0;

    virtual int GetPoints() const = 0;
    virtual int GetHighPoints() const = 0;
    virtual SaveData GetCurrentData() const = 0;
};
