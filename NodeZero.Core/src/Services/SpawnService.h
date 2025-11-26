#pragma once

#include <functional>
#include <vector>

#include "Enums/NodeShape.h"
#include "INode.h"
#include "Services/ISpawnService.h"

class SpawnService : public ISpawnService {
   private:
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_SpawnTimer;
    float m_SpawnInterval;
    int m_CurrentLevel;

    std::function<void(float, float, NodeShape, float, float)> m_OnNodeSpawned;

   public:
    SpawnService();
    ~SpawnService() override = default;

    void Initialize(float screenWidth, float screenHeight);
    void UpdateAutoSpawn(float deltaTime) override;
    void Reset();

    void SetCurrentLevel(int level);
    void SetOnNodeSpawnedCallback(std::function<void(float, float, NodeShape, float, float)> callback);

    NodeShape GetRandomShape() const override;
    float CalculateNodeHP(float baseHP) const override;
    bool ShouldAutoSpawn() const override;

   private:
    float RandomRange(float minValue, float maxValue) const;
    void SpawnRandomNode();
};
