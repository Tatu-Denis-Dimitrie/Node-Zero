#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>

#include "../NodeZero.Core/src/Services/PickupService.h"
#include "../NodeZero.Core/src/Services/DamageZoneService.h"
#include "../NodeZero.Core/src/Node.h"
#include "../NodeZero.Core/include/Config/GameConfig.h"
#include "../NodeZero.Core/include/Enums/NodeShape.h"
#include "../NodeZero.Core/include/Enums/NodeState.h"
#include "../NodeZero.Core/include/Types/PointPickup.h"
#include "../NodeZero.Core/include/Types/Position.h"

// ============================================================================
// PickupService Tests
// ============================================================================
class PickupServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::srand(42);
        pickupService = std::make_unique<PickupService>();
        pickupService->Initialize(600.0f);
    }
    std::unique_ptr<PickupService> pickupService;
};

TEST_F(PickupServiceTest, InitiallyEmpty) {
    EXPECT_EQ(pickupService->GetPickups().size(), 0);
    EXPECT_EQ(pickupService->GetPickupPoints(), 0);
}

TEST_F(PickupServiceTest, SpawnPickupsWorks) {
    Position origin{400.0f, 300.0f};

    pickupService->SpawnPointPickups(origin, 5, 1);

    const auto& pickups = pickupService->GetPickups();
    EXPECT_EQ(pickups.size(), 5);

    for (const auto& pickup : pickups) {
        EXPECT_EQ(pickup.points, 1);
        EXPECT_FLOAT_EQ(pickup.lifetime, GameConfig::PICKUP_LIFETIME);
    }
}

TEST_F(PickupServiceTest, SpawnedPickupsHaveUniqueIds) {
    Position origin{400.0f, 300.0f};

    pickupService->SpawnPointPickups(origin, 10, 1);

    const auto& pickups = pickupService->GetPickups();
    for (size_t i = 0; i < pickups.size(); ++i) {
        for (size_t j = i + 1; j < pickups.size(); ++j) {
            EXPECT_NE(pickups[i].id, pickups[j].id);
        }
    }
}

TEST_F(PickupServiceTest, CollectPickupWorks) {
    Position origin{400.0f, 300.0f};
    pickupService->SpawnPointPickups(origin, 5, 2);

    pickupService->Update(GameConfig::PICKUP_COLLECT_DELAY + 0.01f);

    const auto& pickups = pickupService->GetPickups();
    int pickupId = pickups[0].id;

    bool collected = pickupService->CollectPickup(pickupId);

    EXPECT_TRUE(collected);
    EXPECT_EQ(pickupService->GetPickupPoints(), 2);
}

TEST_F(PickupServiceTest, CollectPickupFailsBeforeDelay) {
    Position origin{400.0f, 300.0f};
    pickupService->SpawnPointPickups(origin, 5, 2);

    const auto& pickups = pickupService->GetPickups();
    int pickupId = pickups[0].id;

    pickupService->Update(0.05f);

    bool collected = pickupService->CollectPickup(pickupId);
    EXPECT_FALSE(collected);
}

TEST_F(PickupServiceTest, UpdateRemovesExpiredPickups) {
    Position origin{400.0f, 300.0f};
    pickupService->SpawnPointPickups(origin, 5, 1);

    size_t initialCount = pickupService->GetPickups().size();

    pickupService->Update(GameConfig::PICKUP_LIFETIME + 1.0f);

    EXPECT_LT(pickupService->GetPickups().size(), initialCount);
}

TEST_F(PickupServiceTest, ResetClearsEverything) {
    Position origin{400.0f, 300.0f};
    pickupService->SpawnPointPickups(origin, 5, 2);

    pickupService->Update(GameConfig::PICKUP_COLLECT_DELAY + 0.01f);
    const auto& pickups = pickupService->GetPickups();
    if (pickups.size() > 0) {
        pickupService->CollectPickup(pickups[0].id);
    }

    pickupService->Reset();

    EXPECT_EQ(pickupService->GetPickups().size(), 0);
    EXPECT_EQ(pickupService->GetPickupPoints(), 0);
}

TEST_F(PickupServiceTest, ProcessCollectionWorks) {
    Position origin{400.0f, 300.0f};
    pickupService->SpawnPointPickups(origin, 5, 1);

    pickupService->Update(GameConfig::PICKUP_COLLECT_DELAY + 0.01f);

    std::vector<PointPickup> collected;
    pickupService->ProcessPickupCollection(400.0f, 300.0f, 100.0f, collected);

    EXPECT_GT(collected.size(), 0);
}

// ============================================================================
// DamageZoneService Tests with Node
// ============================================================================

class DamageZoneServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        damageZoneService = std::make_unique<DamageZoneService>();
    }
    std::unique_ptr<DamageZoneService> damageZoneService;
};

TEST_F(DamageZoneServiceTest, InitiallyNotReadyToDealDamage) {
    EXPECT_FALSE(damageZoneService->ShouldDealDamage());
}

TEST_F(DamageZoneServiceTest, TimerWorks) {
    damageZoneService->UpdateTimer(1.5f);
    EXPECT_TRUE(damageZoneService->ShouldDealDamage());

    damageZoneService->ResetTimer();
    EXPECT_FALSE(damageZoneService->ShouldDealDamage());
}

TEST_F(DamageZoneServiceTest, NodeInZoneTakesDamage) {
    auto node = std::make_unique<Node>(NodeShape::Circle, 30.0f, 0.0f);
    node->SetHP(100.0f);
    node->Spawn(400.0f, 300.0f);
    std::vector<Node*> nodes = {node.get()};

    float healthCost = 0.0f;
    auto onDamaged = [&](Node* n, float cost) {
        healthCost = cost;
    };

    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 1, nodes, onDamaged);

    EXPECT_LT(node->GetHP(), 100.0f);
    EXPECT_GT(healthCost, 0.0f);
}

TEST_F(DamageZoneServiceTest, NodeOutsideZoneTakesNoDamage) {
    auto node = std::make_unique<Node>(NodeShape::Circle, 30.0f, 0.0f);
    node->SetHP(100.0f);
    node->Spawn(400.0f, 300.0f);
    std::vector<Node*> nodes = {node.get()};

    damageZoneService->ProcessDamageZone(1000.0f, 1000.0f, 50.0f, 50.0f, 1, nodes, [](Node*, float){});

    EXPECT_FLOAT_EQ(node->GetHP(), 100.0f);
}

TEST_F(DamageZoneServiceTest, InactiveNodeTakesNoDamage) {
    auto node = std::make_unique<Node>(NodeShape::Circle, 30.0f, 0.0f);
    node->SetHP(100.0f);
    node->Spawn(400.0f, 300.0f);
    node->Kill();

    std::vector<Node*> nodes = {node.get()};

    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 1, nodes, [](Node*, float){});

    EXPECT_FLOAT_EQ(node->GetHP(), 100.0f);
}

TEST_F(DamageZoneServiceTest, HealthCostScalesWithLevel) {
    auto node = std::make_unique<Node>(NodeShape::Circle, 30.0f, 0.0f);
    node->SetHP(100.0f);
    node->Spawn(400.0f, 300.0f);
    std::vector<Node*> nodes = {node.get()};

    float healthCostLevel1 = 0.0f;
    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 1, nodes,
        [&](Node*, float cost) { healthCostLevel1 = cost; });

    node->Spawn(400.0f, 300.0f);

    float healthCostLevel5 = 0.0f;
    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 5, nodes,
        [&](Node*, float cost) { healthCostLevel5 = cost; });

    EXPECT_GT(healthCostLevel5, healthCostLevel1);
}

TEST_F(DamageZoneServiceTest, BossNodeCostsMoreHealth) {
    auto bossNode = std::make_unique<Node>(NodeShape::Boss, 30.0f, 0.0f);
    bossNode->SetHP(200.0f);
    bossNode->Spawn(400.0f, 300.0f);
    std::vector<Node*> nodes = {bossNode.get()};

    float healthCost = 0.0f;
    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 1, nodes,
        [&](Node*, float cost) { healthCost = cost; });

    EXPECT_GT(healthCost, 1.0f);
}

TEST_F(DamageZoneServiceTest, MultipleNodesCanTakeDamage) {
    auto node1 = std::make_unique<Node>(NodeShape::Circle, 30.0f, 0.0f);
    node1->SetHP(100.0f);
    node1->Spawn(400.0f, 300.0f);

    auto node2 = std::make_unique<Node>(NodeShape::Square, 30.0f, 0.0f);
    node2->SetHP(100.0f);
    node2->Spawn(450.0f, 300.0f);

    std::vector<Node*> nodes = {node1.get(), node2.get()};

    int damageCount = 0;
    damageZoneService->ProcessDamageZone(425.0f, 300.0f, 100.0f, 50.0f, 1, nodes,
        [&](Node*, float) { damageCount++; });

    EXPECT_EQ(damageCount, 2);
    EXPECT_LT(node1->GetHP(), 100.0f);
    EXPECT_LT(node2->GetHP(), 100.0f);
}
