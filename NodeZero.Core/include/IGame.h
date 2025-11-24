#pragma once

#include <vector>

#include "Events/ISubject.h"
#include "Types/PointPickup.h"
#include "Types/SaveData.h"

class INode;
class ILevel;

class IGame : public ISubject {
   public:
    virtual ~IGame() = default;

    virtual void Initialize(float screenWidth, float screenHeight) = 0;

    virtual void Update(float deltaTime) = 0;
    virtual void SetMousePosition(float x, float y) = 0;
    virtual std::vector<PointPickup> GetCollectedPickupsThisFrame() const = 0;

    virtual float GetScreenWidth() const = 0;

    virtual float GetScreenHeight() const = 0;

    virtual const std::vector<INode*>& GetNodes() const = 0;
    virtual const std::vector<PointPickup>& GetPickups() const = 0;

    virtual void SpawnNode(float x, float y) = 0;
    virtual void UpdateAutoSpawn(float deltaTime) = 0;
    virtual bool CollectPickup(int pickupId) = 0;
    virtual void ProcessDamageZone(float centerX, float centerY, float zoneSize, float damage, bool shouldDealDamage) = 0;
    virtual std::vector<PointPickup> ProcessPickupCollection(float centerX, float centerY, float zoneSize) = 0;
    virtual void UpdateDamageTimer(float deltaTime) = 0;
    virtual bool ShouldDealDamage() const = 0;
    virtual void ResetDamageTimer() = 0;

    virtual void Reset() = 0;
    virtual bool ShouldGameOver() const = 0;

    virtual int GetPickupScore() const = 0;

    virtual float GetCurrentHealth() const = 0;
    virtual float GetMaxHealth() const = 0;
    virtual void ReduceHealth(float amount) = 0;
    virtual void UpdateHealth(float deltaTime) = 0;
    virtual bool IsGameOver() const = 0;

    virtual int GetNodesDestroyed() const = 0;
    virtual void SaveProgress() = 0;
    virtual int GetHighScore() const = 0;

    virtual bool BuyHealthUpgrade() = 0;
    virtual int GetHealthUpgradeCost() const = 0;

    virtual bool BuyRegenUpgrade() = 0;
    virtual int GetRegenUpgradeCost() const = 0;
    virtual float GetRegenRate() const = 0;

    virtual float GetDamageZoneSize() const = 0;
    virtual bool BuyDamageZoneUpgrade() = 0;
    virtual int GetDamageZoneUpgradeCost() const = 0;

    virtual float GetDamagePerTick() const = 0;
    virtual bool BuyDamageUpgrade() = 0;
    virtual int GetDamageUpgradeCost() const = 0;

    virtual float GetProgressBarPercentage() const = 0;

    virtual int GetCurrentLevel() const = 0;
    virtual int GetNodesDestroyedThisLevel() const = 0;

    virtual bool IsLevelCompleted() const = 0;
    virtual void StartNextLevel() = 0;

    virtual int GetCoins() const = 0;
    virtual SaveData GetSaveData() const = 0;
};
