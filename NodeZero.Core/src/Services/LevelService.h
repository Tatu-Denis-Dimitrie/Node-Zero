#pragma once

#include "Services/ILevelService.h"

class LevelService : public ILevelService {
   private:
    int m_CurrentLevel;
    int m_NodesDestroyedThisLevel;
    bool m_BossActive;
    bool m_LevelCompleted;
    float m_LevelTimer;
    float m_LevelDuration;

   public:
    LevelService();
    ~LevelService() override = default;

    void Initialize(int startLevel) override;
    void Update(float deltaTime) override;
    void Update(float deltaTime, bool bossActive);
    void Reset() override;
    void Reset(int level);

    void IncrementNodesDestroyed() override;
    void SetBossActive(bool active);
    void SetLevelCompleted(bool completed);
    void StartNextLevel() override;

    int GetCurrentLevel() const override;
    int GetNodesDestroyedThisLevel() const override;
    bool IsBossActive() const;
    bool IsLevelCompleted() const override;
    bool ShouldSpawnBoss() const override;
    float GetProgressBarPercentage() const override;
};
