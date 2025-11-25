#pragma once

class ILevelService {
   public:
    virtual ~ILevelService() = default;

    virtual void Initialize(int startLevel) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void StartNextLevel() = 0;
    virtual void IncrementNodesDestroyed() = 0;
    virtual void Reset() = 0;

    virtual int GetCurrentLevel() const = 0;
    virtual int GetNodesDestroyedThisLevel() const = 0;
    virtual float GetProgressBarPercentage() const = 0;
    virtual bool IsLevelCompleted() const = 0;
    virtual bool ShouldSpawnBoss() const = 0;
};
