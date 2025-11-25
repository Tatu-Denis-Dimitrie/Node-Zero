#pragma once

#include <vector>

#include "../Types/PointPickup.h"
#include "../Types/Position.h"

class IPickupService {
   public:
    virtual ~IPickupService() = default;

    virtual void SpawnPointPickups(const Position& origin, int count, int pointValue) = 0;
    virtual bool CollectPickup(int pickupId) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Clear() = 0;

    virtual const std::vector<PointPickup>& GetPickups() const = 0;
    virtual int GetPickupPoints() const = 0;

    virtual void ProcessPickupCollection(float mouseX, float mouseY, float damageZoneSize,
                                         std::vector<PointPickup>& collectedPickups) = 0;
};
