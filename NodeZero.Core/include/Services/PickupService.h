#pragma once

#include <vector>

#include "Types/PointPickup.h"
#include "Types/Position.h"

class PickupService {
   private:
    std::vector<PointPickup> m_Pickups;
    int m_NextPickupId;
    int m_PickupPoints;
    float m_ScreenHeight;

   public:
    PickupService();
    ~PickupService() = default;

    void Initialize(float screenHeight);
    void Update(float deltaTime);
    void Reset();

    void SpawnPointPickups(const Position& origin);
    bool CollectPickup(int pickupId);
    std::vector<PointPickup> ProcessPickupCollection(float centerX, float centerY, float zoneSize);

    const std::vector<PointPickup>& GetPickups() const;
    int GetPickupPoints() const;

   private:
    float RandomRange(float minValue, float maxValue) const;
};
