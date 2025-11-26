#pragma once

class IUpgradeService {
   public:
    virtual ~IUpgradeService() = default;

    virtual bool BuyHealthUpgrade() = 0;
    virtual bool BuyRegenUpgrade() = 0;
    virtual bool BuyDamageZoneUpgrade() = 0;
    virtual bool BuyDamageUpgrade() = 0;

    virtual float GetMaxHealth() const = 0;
    virtual float GetRegenRate() const = 0;
    virtual float GetDamageZoneSize() const = 0;
    virtual float GetDamagePerTick() const = 0;

    virtual int GetHealthUpgradeCost() const = 0;
    virtual int GetRegenUpgradeCost() const = 0;
    virtual int GetDamageZoneUpgradeCost() const = 0;
    virtual int GetDamageUpgradeCost() const = 0;
};
