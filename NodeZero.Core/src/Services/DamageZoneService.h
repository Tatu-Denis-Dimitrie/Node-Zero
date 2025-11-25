#pragma once

#include <functional>
#include <vector>

#include "Services/IDamageZoneService.h"
#include "INode.h"

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

    void ProcessDamageZone(float mouseX, float mouseY, float damageZoneSize,
                          float damagePerTick,
                          std::function<void(int nodeIndex, float damage)> onDamageNode) override;

    void ProcessDamageZone(
        float centerX,
        float centerY,
        float zoneSize,
        float damage,
        int currentLevel,
        const std::vector<INode*>& nodes,
        std::function<void(INode*, float)> onNodeDamaged);
};
