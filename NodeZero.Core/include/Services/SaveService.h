#pragma once

#include "ISaveService.h"
#include "Types/SaveData.h"

class SaveService : public ISaveService {
   private:
    SaveData m_CurrentData;

   public:
    SaveService();
    ~SaveService() override = default;

    void Initialize();

    SaveData LoadProgress() override;
    void SaveProgress(const SaveData& data) override;

    void UpdatePoints(int points) override;
    void UpdateHighPoints(int highPoints) override;
    void UpdateGamesPlayed(int gamesPlayed) override;
    void UpdateTotalNodesDestroyed(int totalNodesDestroyed) override;
    void UpdateStats(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick, int currentLevel) override;

    int GetPoints() const override;
    int GetHighPoints() const override;
    SaveData GetCurrentData() const override;
};
