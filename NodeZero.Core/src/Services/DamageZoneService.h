#pragma once

#include <functional>
#include <vector>

#include "Services/IDamageZoneService.h"

class Node;

class DamageZoneService : public IDamageZoneService {
   private:
    float m_DamageTimer;
    float m_DamageInterval;

   public:
    DamageZoneService();
    ~DamageZoneService() override = default;

    void UpdateTimer(float deltaTime) override;
    void ResetTimer() override;
    bool ShouldDealDamage() const override;

    void ProcessDamageZone(
        float centerX,
        float centerY,
        float zoneSize,
        float damage,
        int currentLevel,
        const std::vector<Node*>& nodes,
        std::function<void(Node*, float)> onNodeDamaged) override;
};
