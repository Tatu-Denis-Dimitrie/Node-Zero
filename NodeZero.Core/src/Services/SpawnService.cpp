#include "SpawnService.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "Config/GameConfig.h"

SpawnService::SpawnService()
    : m_ScreenWidth(0.0f),
      m_ScreenHeight(0.0f),
      m_SpawnTimer(0.0f),
      m_CurrentLevel(1) {
}

void SpawnService::Initialize(float screenWidth, float screenHeight) {
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
}

void SpawnService::UpdateAutoSpawn(float deltaTime) {
    m_SpawnTimer += deltaTime;
}

void SpawnService::ResetSpawnTimer() {
    m_SpawnTimer = 0.0f;
}

void SpawnService::SetCurrentLevel(int level) {
    m_CurrentLevel = level;
}

SpawnInfo SpawnService::GetNextSpawn() const {
    float centerX = m_ScreenWidth / 2.0f;
    float centerY = m_ScreenHeight / 2.0f;

    // Pick random edge to spawn from
    int edge = std::rand() % 4;
    float spawnX, spawnY;

    switch (edge) {
        case 0:  // Top
            spawnX = RandomRange(50.0f, m_ScreenWidth - 50.0f);
            spawnY = -50.0f;
            break;
        case 1:  // Right
            spawnX = m_ScreenWidth + 50.0f;
            spawnY = RandomRange(50.0f, m_ScreenHeight - 50.0f);
            break;
        case 2:  // Bottom
            spawnX = RandomRange(50.0f, m_ScreenWidth - 50.0f);
            spawnY = m_ScreenHeight + 50.0f;
            break;
        case 3:  // Left
            spawnX = -50.0f;
            spawnY = RandomRange(50.0f, m_ScreenHeight - 50.0f);
            break;
        default:
            spawnX = centerX;
            spawnY = -50.0f;
            break;
    }

    // Calculate direction towards center (with some randomness)
    float offsetRange = 150.0f;
    float targetX = centerX + RandomRange(-offsetRange, offsetRange);
    float targetY = centerY + RandomRange(-offsetRange, offsetRange);

    float dirX = targetX - spawnX;
    float dirY = targetY - spawnY;

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0.0f) {
        dirX /= length;
        dirY /= length;
    }

    return SpawnInfo{
        Position{spawnX, spawnY},
        GetRandomShape(),
        dirX,
        dirY
    };
}

NodeShape SpawnService::GetRandomShape() const {
    int chance = std::rand() % 100;

    if (chance < 60) {
        return NodeShape::Square;
    } else if (chance < 90) {
        return NodeShape::Circle;
    } else {
        return NodeShape::Hexagon;
    }
}

float SpawnService::CalculateNodeHP(float baseHP) const {
    return baseHP * (1.0f + (m_CurrentLevel - 1) * 0.2f);
}

float SpawnService::RandomRange(float minValue, float maxValue) const {
    float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return minValue + (maxValue - minValue) * t;
}

bool SpawnService::ShouldAutoSpawn() const {
    float currentSpawnInterval = std::max(0.5f, 2.0f - (m_CurrentLevel - 1) * 0.15f);
    return m_SpawnTimer >= currentSpawnInterval;
}
