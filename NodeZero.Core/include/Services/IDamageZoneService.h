#pragma once

#include <functional>
#include <vector>

class Node;

class IDamageZoneService {
   public:
    virtual ~IDamageZoneService() = default;

    virtual void ProcessDamageZone(
        float centerX,
        float centerY,
        float zoneSize,
        float damage,
        int currentLevel,
        const std::vector<Node*>& nodes,
        std::function<void(Node*, float)> onNodeDamaged) = 0;

    virtual void UpdateTimer(float deltaTime) = 0;
    virtual void ResetTimer() = 0;
    virtual bool ShouldDealDamage() const = 0;
};
