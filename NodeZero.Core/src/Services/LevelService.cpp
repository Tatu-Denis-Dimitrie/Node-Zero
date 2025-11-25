#include "Services/LevelService.h"

#include "Config/GameConfig.h"

LevelService::LevelService()
    : m_CurrentLevel(1),
      m_NodesDestroyedThisLevel(0),
      m_BossActive(false),
      m_LevelCompleted(false),
      m_LevelTimer(0.0f),
      m_LevelDuration(GameConfig::LEVEL_DURATION) {
}

void LevelService::Initialize(int startLevel) {
    m_CurrentLevel = startLevel;
    if (m_CurrentLevel < 1) {
        m_CurrentLevel = 1;
    }
}

void LevelService::Update(float deltaTime, bool bossActive) {
    if (!bossActive) {
        m_LevelTimer += deltaTime;
    }
}

void LevelService::Reset(int level) {
    m_CurrentLevel = level;
    if (m_CurrentLevel < 1) {
        m_CurrentLevel = 1;
    }
    m_NodesDestroyedThisLevel = 0;
    m_BossActive = false;
    m_LevelCompleted = false;
    m_LevelTimer = 0.0f;
}

void LevelService::IncrementNodesDestroyed() {
    m_NodesDestroyedThisLevel++;
}

void LevelService::SetBossActive(bool active) {
    m_BossActive = active;
}

void LevelService::SetLevelCompleted(bool completed) {
    m_LevelCompleted = completed;
}

void LevelService::StartNextLevel() {
    m_CurrentLevel++;
    m_NodesDestroyedThisLevel = 0;
    m_LevelCompleted = false;
    m_LevelTimer = 0.0f;
    m_BossActive = false;
}

int LevelService::GetCurrentLevel() const {
    return m_CurrentLevel;
}

int LevelService::GetNodesDestroyedThisLevel() const {
    return m_NodesDestroyedThisLevel;
}

bool LevelService::IsBossActive() const {
    return m_BossActive;
}

bool LevelService::IsLevelCompleted() const {
    return m_LevelCompleted;
}

bool LevelService::ShouldSpawnBoss() const {
    return m_LevelTimer >= m_LevelDuration && !m_BossActive;
}

float LevelService::GetProgressBarPercentage() const {
    float percentage = (m_LevelTimer / m_LevelDuration) * 100.0f;
    if (percentage > 100.0f) {
        percentage = 100.0f;
    }
    return percentage;
}
void LevelService::Update(float deltaTime) {
    Update(deltaTime, m_BossActive);
}

void LevelService::Reset() {
    Reset(1);
}
