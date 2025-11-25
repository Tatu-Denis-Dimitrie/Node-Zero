#pragma once

class LevelService {
   private:
    int m_CurrentLevel;
    int m_NodesDestroyedThisLevel;
    bool m_BossActive;
    bool m_LevelCompleted;
    float m_LevelTimer;
    float m_LevelDuration;

   public:
    LevelService();
    ~LevelService() = default;

    void Initialize(int startLevel);
    void Update(float deltaTime, bool bossActive);
    void Reset(int level);

    void IncrementNodesDestroyed();
    void SetBossActive(bool active);
    void SetLevelCompleted(bool completed);
    void StartNextLevel();

    int GetCurrentLevel() const;
    int GetNodesDestroyedThisLevel() const;
    bool IsBossActive() const;
    bool IsLevelCompleted() const;
    bool ShouldSpawnBoss() const;
    float GetProgressBarPercentage() const;
};
