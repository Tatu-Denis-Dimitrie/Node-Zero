#pragma once

#include "Enums/NodeShape.h"
#include "Services/ISpawnService.h"

class SpawnService : public ISpawnService {
   private:
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_SpawnTimer;
    int m_CurrentLevel;

   public:
    SpawnService();
    ~SpawnService() override = default;

    void Initialize(float screenWidth, float screenHeight);
    void UpdateAutoSpawn(float deltaTime) override;
    void ResetSpawnTimer();

    void SetCurrentLevel(int level);

    SpawnInfo GetNextSpawn() const override;
    float CalculateNodeHP(float baseHP) const override;
    bool ShouldAutoSpawn() const override;

   private:
    float RandomRange(float minValue, float maxValue) const;
    NodeShape GetRandomShape() const;
};
