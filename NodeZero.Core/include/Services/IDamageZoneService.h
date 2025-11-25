#pragma once

#include <functional>

class IDamageZoneService {
   public:
    virtual ~IDamageZoneService() = default;

    virtual void ProcessDamageZone(float mouseX, float mouseY, float damageZoneSize,
                                    float damagePerTick,
                                    std::function<void(int nodeIndex, float damage)> onDamageNode) = 0;

    virtual void UpdateTimer(float deltaTime) = 0;
    virtual void ResetTimer() = 0;
    virtual bool ShouldDealDamage() const = 0;
};
