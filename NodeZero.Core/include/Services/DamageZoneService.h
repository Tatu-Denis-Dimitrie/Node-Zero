#pragma once

#include <functional>
#include <vector>

#include "INode.h"

class DamageZoneService {
   private:
    float m_DamageTimer;
    float m_DamageInterval;

   public:
    DamageZoneService();
    ~DamageZoneService() = default;

    void UpdateTimer(float deltaTime);
    void ResetTimer();
    bool ShouldDealDamage() const;

    void ProcessDamageZone(
        float centerX,
        float centerY,
        float zoneSize,
        float damage,
        int currentLevel,
        const std::vector<INode*>& nodes,
        std::function<void(INode*, float)> onNodeDamaged);
};
