#pragma once

#include <vector>

#include "Types/PointPickup.h"

class INode;
class ILevel;
class EventManager;

class IGame {
   public:
    virtual ~IGame() = default;

    virtual void Initialize(float screenWidth, float screenHeight) = 0;

    virtual void Update(float deltaTime) = 0;

    virtual float GetScreenWidth() const = 0;

    virtual float GetScreenHeight() const = 0;

    virtual const std::vector<INode*>& GetNodes() const = 0;
    virtual const std::vector<PointPickup>& GetPickups() const = 0;

    virtual void SpawnNode(float x, float y) = 0;
    virtual bool CollectPickup(int pickupId) = 0;

    virtual EventManager& GetEventManager() = 0;

    virtual void Reset() = 0;

    virtual int GetPickupScore() const = 0;

    // Health system
    virtual float GetCurrentHealth() const = 0;
    virtual float GetMaxHealth() const = 0;
    virtual void ReduceHealth(float amount) = 0;
    virtual void UpdateHealth(float deltaTime) = 0;
    virtual bool IsGameOver() const = 0;

    // Save/Load system
    virtual int GetNodesDestroyed() const = 0;
    virtual void SaveProgress() = 0;
    virtual int GetHighScore() const = 0;

    // Upgrade system
    virtual bool BuyHealthUpgrade() = 0;
    virtual int GetHealthUpgradeCost() const = 0;
};
