#pragma once

class UpgradeService {
   private:
    float m_MaxHealth;
    float m_RegenRate;
    float m_DamageZoneSize;
    float m_DamagePerTick;

   public:
    UpgradeService();
    ~UpgradeService() = default;

    void Initialize(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick);

    bool BuyHealthUpgrade();
    int GetHealthUpgradeCost() const;
    float GetMaxHealth() const;

    bool BuyRegenUpgrade();
    int GetRegenUpgradeCost() const;
    float GetRegenRate() const;

    bool BuyDamageZoneUpgrade();
    int GetDamageZoneUpgradeCost() const;
    float GetDamageZoneSize() const;

    bool BuyDamageUpgrade();
    int GetDamageUpgradeCost() const;
    float GetDamagePerTick() const;
};
