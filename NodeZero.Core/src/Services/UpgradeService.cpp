#include "Services/UpgradeService.h"

#include "Config/GameConfig.h"
#include "Systems/SaveSystem.h"

UpgradeService::UpgradeService()
    : m_MaxHealth(GameConfig::HEALTH_DEFAULT),
      m_RegenRate(0.0f),
      m_DamageZoneSize(GameConfig::DAMAGE_ZONE_DEFAULT_SIZE),
      m_DamagePerTick(GameConfig::DAMAGE_PER_TICK_DEFAULT) {
}

void UpgradeService::Initialize(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick) {
    m_MaxHealth = maxHealth;
    m_RegenRate = regenRate;
    m_DamageZoneSize = damageZoneSize;
    m_DamagePerTick = damagePerTick;
}

bool UpgradeService::BuyHealthUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.points < GameConfig::HEALTH_UPGRADE_COST) {
        return false;
    }

    saveData.points -= GameConfig::HEALTH_UPGRADE_COST;
    m_MaxHealth += 1.0f;
    saveData.maxHealth = m_MaxHealth;

    SaveSystem::SaveProgress(saveData);
    return true;
}

int UpgradeService::GetHealthUpgradeCost() const {
    return GameConfig::HEALTH_UPGRADE_COST;
}

float UpgradeService::GetMaxHealth() const {
    return m_MaxHealth;
}

bool UpgradeService::BuyRegenUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.points < GameConfig::REGEN_UPGRADE_COST) {
        return false;
    }

    saveData.points -= GameConfig::REGEN_UPGRADE_COST;
    m_RegenRate += GameConfig::REGEN_UPGRADE_AMOUNT;
    saveData.regenRate = m_RegenRate;

    SaveSystem::SaveProgress(saveData);
    return true;
}

int UpgradeService::GetRegenUpgradeCost() const {
    return GameConfig::REGEN_UPGRADE_COST;
}

float UpgradeService::GetRegenRate() const {
    return m_RegenRate;
}

bool UpgradeService::BuyDamageZoneUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.points < GameConfig::DAMAGE_ZONE_UPGRADE_COST) {
        return false;
    }

    if (m_DamageZoneSize >= GameConfig::DAMAGE_ZONE_MAX_SIZE) {
        return false;
    }

    saveData.points -= GameConfig::DAMAGE_ZONE_UPGRADE_COST;
    m_DamageZoneSize += GameConfig::DAMAGE_ZONE_UPGRADE_AMOUNT;

    if (m_DamageZoneSize > GameConfig::DAMAGE_ZONE_MAX_SIZE) {
        m_DamageZoneSize = GameConfig::DAMAGE_ZONE_MAX_SIZE;
    }

    saveData.damageZoneSize = m_DamageZoneSize;
    SaveSystem::SaveProgress(saveData);
    return true;
}

int UpgradeService::GetDamageZoneUpgradeCost() const {
    return GameConfig::DAMAGE_ZONE_UPGRADE_COST;
}

float UpgradeService::GetDamageZoneSize() const {
    return m_DamageZoneSize;
}

bool UpgradeService::BuyDamageUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.points < GameConfig::DAMAGE_UPGRADE_COST) {
        return false;
    }

    saveData.points -= GameConfig::DAMAGE_UPGRADE_COST;
    m_DamagePerTick += GameConfig::DAMAGE_UPGRADE_AMOUNT;
    saveData.damagePerTick = m_DamagePerTick;

    SaveSystem::SaveProgress(saveData);
    return true;
}

int UpgradeService::GetDamageUpgradeCost() const {
    return GameConfig::DAMAGE_UPGRADE_COST;
}

float UpgradeService::GetDamagePerTick() const {
    return m_DamagePerTick;
}
