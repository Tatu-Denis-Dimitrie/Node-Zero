#include "Services/SpawnService.h"

#include <cmath>
#include <cstdlib>

#include "Config/GameConfig.h"

SpawnService::SpawnService()
    : m_ScreenWidth(0.0f),
      m_ScreenHeight(0.0f),
      m_SpawnTimer(0.0f),
      m_SpawnInterval(2.0f),
      m_CurrentLevel(1) {
}

void SpawnService::Initialize(float screenWidth, float screenHeight) {
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
}

void SpawnService::UpdateAutoSpawn(float deltaTime) {
    m_SpawnTimer += deltaTime;

    float currentSpawnInterval = std::max(0.5f, 2.0f - (m_CurrentLevel - 1) * 0.15f);

    if (m_SpawnTimer >= currentSpawnInterval) {
        m_SpawnTimer = 0.0f;
        SpawnRandomNode();
    }
}

void SpawnService::Reset() {
    m_SpawnTimer = 0.0f;
}

void SpawnService::SetCurrentLevel(int level) {
    m_CurrentLevel = level;
}

void SpawnService::SetOnNodeSpawnedCallback(std::function<void(float, float, NodeShape, float, float)> callback) {
    m_OnNodeSpawned = callback;
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

void SpawnService::SpawnRandomNode() {
    float centerX = m_ScreenWidth / 2.0f;
    float centerY = m_ScreenHeight / 2.0f;

    int edge = std::rand() % 4;
    float spawnX, spawnY;

    switch (edge) {
        case 0:
            spawnX = RandomRange(50.0f, m_ScreenWidth - 50.0f);
            spawnY = -50.0f;
            break;
        case 1:
            spawnX = m_ScreenWidth + 50.0f;
            spawnY = RandomRange(50.0f, m_ScreenHeight - 50.0f);
            break;
        case 2:
            spawnX = RandomRange(50.0f, m_ScreenWidth - 50.0f);
            spawnY = m_ScreenHeight + 50.0f;
            break;
        case 3:
            spawnX = -50.0f;
            spawnY = RandomRange(50.0f, m_ScreenHeight - 50.0f);
            break;
        default:
            spawnX = centerX;
            spawnY = -50.0f;
            break;
    }

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

    NodeShape shape = GetRandomShape();
    float nodeSize = m_ScreenHeight * 0.0375f;

    if (m_OnNodeSpawned) {
        m_OnNodeSpawned(spawnX, spawnY, shape, dirX, dirY);
    }
}
void SpawnService::ResetAutoSpawnTimer() {
    m_SpawnTimer = 0.0f;
}

bool SpawnService::ShouldAutoSpawn() const {
    float currentSpawnInterval = std::max(0.5f, 2.0f - (m_CurrentLevel - 1) * 0.15f);
    return m_SpawnTimer >= currentSpawnInterval;
}
