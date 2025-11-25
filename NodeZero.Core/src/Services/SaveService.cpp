#include "Services/SaveService.h"

#include "Systems/SaveSystem.h"

SaveService::SaveService() {
    m_CurrentData = SaveSystem::LoadProgress();
}

void SaveService::Initialize() {
    m_CurrentData = SaveSystem::LoadProgress();
}

SaveData SaveService::LoadProgress() {
    m_CurrentData = SaveSystem::LoadProgress();
    return m_CurrentData;
}

void SaveService::SaveProgress(const SaveData& data) {
    m_CurrentData = data;
    SaveSystem::SaveProgress(data);
}

void SaveService::UpdatePoints(int points) {
    m_CurrentData.points = points;
    SaveSystem::SaveProgress(m_CurrentData);
}

void SaveService::UpdateHighPoints(int highPoints) {
    m_CurrentData.highPoints = highPoints;
    SaveSystem::SaveProgress(m_CurrentData);
}

void SaveService::UpdateGamesPlayed(int gamesPlayed) {
    m_CurrentData.gamesPlayed = gamesPlayed;
    SaveSystem::SaveProgress(m_CurrentData);
}

void SaveService::UpdateTotalNodesDestroyed(int totalNodesDestroyed) {
    m_CurrentData.totalNodesDestroyed = totalNodesDestroyed;
    SaveSystem::SaveProgress(m_CurrentData);
}

void SaveService::UpdateStats(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick, int currentLevel) {
    m_CurrentData.maxHealth = maxHealth;
    m_CurrentData.regenRate = regenRate;
    m_CurrentData.damageZoneSize = damageZoneSize;
    m_CurrentData.damagePerTick = damagePerTick;
    m_CurrentData.currentLevel = currentLevel;
    SaveSystem::SaveProgress(m_CurrentData);
}

int SaveService::GetPoints() const {
    return m_CurrentData.points;
}

int SaveService::GetHighPoints() const {
    return m_CurrentData.highPoints;
}

SaveData SaveService::GetCurrentData() const {
    return m_CurrentData;
}
