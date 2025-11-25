#pragma once

#include "IUpgradeService.h"

class UpgradeService : public IUpgradeService {
   private:
    float m_MaxHealth;
    float m_RegenRate;
    float m_DamageZoneSize;
    float m_DamagePerTick;

   public:
    UpgradeService();
    ~UpgradeService() override = default;

    void Initialize(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick);

    bool BuyHealthUpgrade() override;
    int GetHealthUpgradeCost() const override;
    float GetMaxHealth() const override;

    bool BuyRegenUpgrade() override;
    int GetRegenUpgradeCost() const override;
    float GetRegenRate() const override;

    bool BuyDamageZoneUpgrade() override;
    int GetDamageZoneUpgradeCost() const override;
    float GetDamageZoneSize() const override;

    bool BuyDamageUpgrade() override;
    int GetDamageUpgradeCost() const override;
    float GetDamagePerTick() const override;

    void LoadFromSave() override;
};
