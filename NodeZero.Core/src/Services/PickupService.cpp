#include "PickupService.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "Config/GameConfig.h"

PickupService::PickupService()
    : m_NextPickupId(0),
      m_PickupPoints(0),
      m_ScreenHeight(0.0f) {
}

void PickupService::Initialize(float screenHeight) {
    m_ScreenHeight = screenHeight;
}

void PickupService::Update(float deltaTime) {
    for (auto& pickup : m_Pickups) {
        pickup.remainingTime -= deltaTime;
    }

    m_Pickups.erase(std::remove_if(m_Pickups.begin(), m_Pickups.end(),
                                   [](const PointPickup& pickup) {
                                       return pickup.remainingTime <= 0.0f;
                                   }),
                    m_Pickups.end());
}

void PickupService::Reset() {
    m_Pickups.clear();
    m_NextPickupId = 0;
    m_PickupPoints = 0;
}

void PickupService::SpawnPointPickups(const Position& origin) {
    int pickupCount = 5 + (std::rand() % 6);

    for (int i = 0; i < pickupCount; ++i) {
        float angle = RandomRange(0.0f, 6.28318530718f);
        float radius = RandomRange(m_ScreenHeight * 0.0125f, m_ScreenHeight * 0.05f);

        PointPickup pickup{};
        pickup.id = m_NextPickupId++;
        pickup.position.x = origin.x + std::cos(angle) * radius;
        pickup.position.y = origin.y + std::sin(angle) * radius;
        pickup.spawnOrigin = origin;
        pickup.size = m_ScreenHeight * 0.0075f;
        pickup.lifetime = GameConfig::PICKUP_LIFETIME;
        pickup.remainingTime = GameConfig::PICKUP_LIFETIME;
        pickup.points = 1;

        m_Pickups.push_back(pickup);
    }
}

bool PickupService::CollectPickup(int pickupId) {
    auto it = std::find_if(m_Pickups.begin(), m_Pickups.end(),
                           [pickupId](const PointPickup& pickup) {
                               return pickup.id == pickupId;
                           });

    if (it == m_Pickups.end()) {
        return false;
    }

    float age = it->GetAge();
    if (age < GameConfig::PICKUP_COLLECT_DELAY) {
        return false;
    }

    m_PickupPoints += it->points;
    m_Pickups.erase(it);
    return true;
}

std::vector<PointPickup> PickupService::ProcessPickupCollection(float centerX, float centerY, float zoneSize) {
    float collectRectX = centerX - zoneSize / 2.0f;
    float collectRectY = centerY - zoneSize / 2.0f;

    std::vector<PointPickup> collectedPickups;

    for (const PointPickup& pickup : m_Pickups) {
        if (pickup.GetAge() < GameConfig::PICKUP_COLLECT_DELAY) {
            continue;
        }

        bool intersects = !(pickup.position.x + pickup.size < collectRectX ||
                            pickup.position.x - pickup.size > collectRectX + zoneSize ||
                            pickup.position.y + pickup.size < collectRectY ||
                            pickup.position.y - pickup.size > collectRectY + zoneSize);

        if (intersects) {
            collectedPickups.push_back(pickup);
            CollectPickup(pickup.id);
        }
    }

    return collectedPickups;
}

const std::vector<PointPickup>& PickupService::GetPickups() const {
    return m_Pickups;
}

int PickupService::GetPickupPoints() const {
    return m_PickupPoints;
}

float PickupService::RandomRange(float minValue, float maxValue) const {
    float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return minValue + (maxValue - minValue) * t;
}
void PickupService::SpawnPointPickups(const Position& origin, int count, int pointValue) {
    for (int i = 0; i < count; ++i) {
        float angle = RandomRange(0.0f, 6.28318530718f);
        float radius = RandomRange(m_ScreenHeight * 0.0125f, m_ScreenHeight * 0.05f);

        PointPickup pickup{};
        pickup.id = m_NextPickupId++;
        pickup.position.x = origin.x + std::cos(angle) * radius;
        pickup.position.y = origin.y + std::sin(angle) * radius;
        pickup.spawnOrigin = origin;
        pickup.size = m_ScreenHeight * 0.0075f;
        pickup.lifetime = GameConfig::PICKUP_LIFETIME;
        pickup.remainingTime = GameConfig::PICKUP_LIFETIME;
        pickup.points = pointValue;

        m_Pickups.push_back(pickup);
    }
}

void PickupService::ProcessPickupCollection(float mouseX, float mouseY, float damageZoneSize,
                                           std::vector<PointPickup>& collectedPickups) {
    collectedPickups.clear();
    
    float collectRectX = mouseX - damageZoneSize / 2.0f;
    float collectRectY = mouseY - damageZoneSize / 2.0f;

    for (const PointPickup& pickup : m_Pickups) {
        if (pickup.GetAge() < GameConfig::PICKUP_COLLECT_DELAY) {
            continue;
        }

        bool intersects = !(pickup.position.x + pickup.size < collectRectX ||
                            pickup.position.x - pickup.size > collectRectX + damageZoneSize ||
                            pickup.position.y + pickup.size < collectRectY ||
                            pickup.position.y - pickup.size > collectRectY + damageZoneSize);

        if (intersects) {
            collectedPickups.push_back(pickup);
            CollectPickup(pickup.id);
        }
    }
}

void PickupService::Clear() {
    Reset();
}
