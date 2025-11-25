#pragma once

#include <vector>

#include "IPickupService.h"
#include "Types/PointPickup.h"
#include "Types/Position.h"

class PickupService : public IPickupService {
   private:
    std::vector<PointPickup> m_Pickups;
    int m_NextPickupId;
    int m_PickupPoints;
    float m_ScreenHeight;

   public:
    PickupService();
    ~PickupService() override = default;

    void Initialize(float screenHeight);
    void Update(float deltaTime) override;
    void Clear() override;
    void Reset();

    void SpawnPointPickups(const Position& origin, int count, int pointValue) override;
    void SpawnPointPickups(const Position& origin);
    bool CollectPickup(int pickupId) override;
    std::vector<PointPickup> ProcessPickupCollection(float centerX, float centerY, float zoneSize);
    void ProcessPickupCollection(float mouseX, float mouseY, float damageZoneSize,
                                 std::vector<PointPickup>& collectedPickups) override;

    const std::vector<PointPickup>& GetPickups() const override;
    int GetPickupPoints() const override;

   private:
    float RandomRange(float minValue, float maxValue) const;
};
