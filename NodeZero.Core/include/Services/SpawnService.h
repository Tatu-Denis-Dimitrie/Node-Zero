#pragma once

#include <functional>
#include <vector>

#include "Enums/NodeShape.h"
#include "INode.h"

class SpawnService {
   private:
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_SpawnTimer;
    float m_SpawnInterval;
    int m_CurrentLevel;

    std::function<void(float, float, NodeShape, float, float)> m_OnNodeSpawned;

   public:
    SpawnService();
    ~SpawnService() = default;

    void Initialize(float screenWidth, float screenHeight);
    void UpdateAutoSpawn(float deltaTime);
    void Reset();

    void SetCurrentLevel(int level);
    void SetOnNodeSpawnedCallback(std::function<void(float, float, NodeShape, float, float)> callback);

    NodeShape GetRandomShape() const;
    float CalculateNodeHP(float baseHP) const;

   private:
    float RandomRange(float minValue, float maxValue) const;
    void SpawnRandomNode();
};
