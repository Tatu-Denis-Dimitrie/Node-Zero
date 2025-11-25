#pragma once

#include <vector>

#include "Events/ISubject.h"
#include "Types/PointPickup.h"

class INode;
class IUpgradeService;
class IPickupService;
class IHealthService;
class ILevelService;
class IDamageZoneService;
class ISpawnService;
class ISaveService;

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

    virtual IUpgradeService& GetUpgradeService() = 0;
    virtual IPickupService& GetPickupService() = 0;
    virtual IHealthService& GetHealthService() = 0;
    virtual ILevelService& GetLevelService() = 0;
    virtual IDamageZoneService& GetDamageZoneService() = 0;
    virtual ISpawnService& GetSpawnService() = 0;
    virtual ISaveService& GetSaveService() = 0;
};
