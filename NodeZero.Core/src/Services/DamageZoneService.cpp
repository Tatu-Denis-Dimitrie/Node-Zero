#include "DamageZoneService.h"

#include <algorithm>
#include <cmath>

#include "Enums/NodeShape.h"
#include "Enums/NodeState.h"
#include "Node.h"

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
    const std::vector<Node*>& nodes,
    std::function<void(Node*, float)> onNodeDamaged) {

    float damageRectX = centerX - zoneSize / 2.0f;
    float damageRectY = centerY - zoneSize / 2.0f;
    float damageRectRight = damageRectX + zoneSize;
    float damageRectBottom = damageRectY + zoneSize;

    for (Node* node : nodes) {
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