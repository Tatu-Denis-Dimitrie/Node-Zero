#include "Services/DamageZoneService.h"

#include <algorithm>
#include <cmath>

#include "Enums/NodeShape.h"
#include "Enums/NodeState.h"

DamageZoneService::DamageZoneService()
    : m_DamageTimer(0.0f),
      m_DamageInterval(1.5f) {
}

void DamageZoneService::UpdateTimer(float deltaTime) {
    m_DamageTimer += deltaTime;
}

void DamageZoneService::ResetTimer() {
    m_DamageTimer = 0.0f;
}

bool DamageZoneService::ShouldDealDamage() const {
    return m_DamageTimer >= m_DamageInterval;
}

void DamageZoneService::ProcessDamageZone(
    float centerX,
    float centerY,
    float zoneSize,
    float damage,
    int currentLevel,
    const std::vector<INode*>& nodes,
    std::function<void(INode*, float)> onNodeDamaged) {

    float damageRectX = centerX - zoneSize / 2.0f;
    float damageRectY = centerY - zoneSize / 2.0f;
    float damageRectRight = damageRectX + zoneSize;
    float damageRectBottom = damageRectY + zoneSize;

    for (INode* node : nodes) {
        if (node->GetState() != NodeState::Active)
            continue;

        float nodeX = node->GetPosition().x;
        float nodeY = node->GetPosition().y;
        float nodeSize = node->GetSize();

        float boundingRadius = nodeSize;
        NodeShape shape = node->GetShape();
        if (shape == NodeShape::Square) {
            boundingRadius = nodeSize * 1.414f;
        } else if (shape == NodeShape::Boss) {
            boundingRadius = nodeSize * 1.414f;
        }

        float closestX = std::max(damageRectX, std::min(nodeX, damageRectRight));
        float closestY = std::max(damageRectY, std::min(nodeY, damageRectBottom));

        float deltaX = nodeX - closestX;
        float deltaY = nodeY - closestY;
        float distanceSquared = deltaX * deltaX + deltaY * deltaY;

        bool inDamageZone = distanceSquared <= (boundingRadius * boundingRadius);

        if (inDamageZone) {
            node->TakeDamage(damage);

            float healthCost = 0.5f;
            if (node->GetShape() == NodeShape::Boss) {
                healthCost *= 8.0f;
            }

            float scaledHealthCost = healthCost * (1.0f + (currentLevel - 1) * 0.20f);

            if (onNodeDamaged) {
                onNodeDamaged(node, scaledHealthCost);
            }
        }
    }
}
void DamageZoneService::ProcessDamageZone(float mouseX, float mouseY, float damageZoneSize,
                                         float damagePerTick,
                                         std::function<void(int nodeIndex, float damage)> onDamageNode) {
    // Simplified interface implementation - actual logic is in the other overload
    // This is here to satisfy the interface but isn't actively used
    if (onDamageNode) {
        // Placeholder - real implementation uses the other overload with full node access
    }
}
