#pragma once

#include <vector>

#include "Events/ISubject.h"
#include "Types/PointPickup.h"

class INode;
class UpgradeService;
class PickupService;
class HealthService;
class LevelService;
class DamageZoneService;
class SpawnService;

class IGame : public ISubject {
   public:
    virtual ~IGame() = default;

    virtual void Initialize(float screenWidth, float screenHeight) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Reset() = 0;
    virtual void StartNextLevel() = 0;

    virtual void SetMousePosition(float x, float y) = 0;
    virtual void SpawnNode(float x, float y) = 0;

    virtual float GetScreenWidth() const = 0;
    virtual float GetScreenHeight() const = 0;

    virtual const std::vector<INode*>& GetNodes() const = 0;
    virtual std::vector<PointPickup> GetCollectedPickupsThisFrame() const = 0;

    virtual int GetNodesDestroyed() const = 0;
    virtual void SaveProgress() = 0;
    virtual int GetHighPoints() const = 0;

    virtual UpgradeService& GetUpgradeService() = 0;
    virtual PickupService& GetPickupService() = 0;
    virtual HealthService& GetHealthService() = 0;
    virtual LevelService& GetLevelService() = 0;
    virtual DamageZoneService& GetDamageZoneService() = 0;
    virtual SpawnService& GetSpawnService() = 0;
};
