#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>

#include "../NodeZero.Core/src/Services/PickupService.h"
#include "../NodeZero.Core/src/Services/DamageZoneService.h"
#include "../NodeZero.Core/include/INode.h"
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
// DamageZoneService Tests with Mock Node
// ============================================================================
class MockNode : public INode {
private:
    Position m_Position;
    NodeShape m_Shape;
    NodeState m_State;
    float m_Size;
    float m_HP;
    float m_MaxHP;

public:
    MockNode(float x, float y, NodeShape shape, float size, float hp)
        : m_Position{x, y}, m_Shape(shape), m_State(NodeState::Active), m_Size(size), m_HP(hp), m_MaxHP(hp) {}

    Position& GetPosition() override { return m_Position; }
    const Position& GetPosition() const override { return m_Position; }
    NodeShape GetShape() const override { return m_Shape; }
    NodeState GetState() const override { return m_State; }
    float GetSize() const override { return m_Size; }
    float GetSpeed() const override { return 0.0f; }
    float GetRotation() const override { return 0.0f; }
    float GetHP() const override { return m_HP; }
    float GetMaxHP() const override { return m_MaxHP; }

    void TakeDamage(float damage) override {
        m_HP -= damage;
        if (m_HP < 0.0f) {
            m_HP = 0.0f;
            m_State = NodeState::Dead;
        }
    }

    void Spawn(float x, float y) override {
        m_Position.x = x;
        m_Position.y = y;
        m_State = NodeState::Active;
    }

    void SetDirection(float dirX, float dirY) override {}
    void Kill() override { m_State = NodeState::Dead; }
    void Update(float deltaTime) override {}
    void SetState(NodeState state) { m_State = state; }
};

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
    auto node = std::make_unique<MockNode>(400.0f, 300.0f, NodeShape::Circle, 30.0f, 100.0f);
    std::vector<INode*> nodes = {node.get()};

    float healthCost = 0.0f;
    auto onDamaged = [&](INode* n, float cost) {
        healthCost = cost;
    };

    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 1, nodes, onDamaged);

    EXPECT_LT(node->GetHP(), 100.0f);
    EXPECT_GT(healthCost, 0.0f);
}

TEST_F(DamageZoneServiceTest, NodeOutsideZoneTakesNoDamage) {
    auto node = std::make_unique<MockNode>(400.0f, 300.0f, NodeShape::Circle, 30.0f, 100.0f);
    std::vector<INode*> nodes = {node.get()};

    damageZoneService->ProcessDamageZone(1000.0f, 1000.0f, 50.0f, 50.0f, 1, nodes, [](INode*, float){});

    EXPECT_FLOAT_EQ(node->GetHP(), 100.0f);
}

TEST_F(DamageZoneServiceTest, InactiveNodeTakesNoDamage) {
    auto node = std::make_unique<MockNode>(400.0f, 300.0f, NodeShape::Circle, 30.0f, 100.0f);
    node->SetState(NodeState::Dead);

    std::vector<INode*> nodes = {node.get()};

    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 1, nodes, [](INode*, float){});

    EXPECT_FLOAT_EQ(node->GetHP(), 100.0f);
}

TEST_F(DamageZoneServiceTest, HealthCostScalesWithLevel) {
    auto node = std::make_unique<MockNode>(400.0f, 300.0f, NodeShape::Circle, 30.0f, 100.0f);
    std::vector<INode*> nodes = {node.get()};

    float healthCostLevel1 = 0.0f;
    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 1, nodes,
        [&](INode*, float cost) { healthCostLevel1 = cost; });

    node->Spawn(400.0f, 300.0f);

    float healthCostLevel5 = 0.0f;
    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 5, nodes,
        [&](INode*, float cost) { healthCostLevel5 = cost; });

    EXPECT_GT(healthCostLevel5, healthCostLevel1);
}

TEST_F(DamageZoneServiceTest, BossNodeCostsMoreHealth) {
    auto bossNode = std::make_unique<MockNode>(400.0f, 300.0f, NodeShape::Boss, 30.0f, 200.0f);
    std::vector<INode*> nodes = {bossNode.get()};

    float healthCost = 0.0f;
    damageZoneService->ProcessDamageZone(400.0f, 300.0f, 100.0f, 50.0f, 1, nodes,
        [&](INode*, float cost) { healthCost = cost; });

    EXPECT_GT(healthCost, 1.0f);
}

TEST_F(DamageZoneServiceTest, MultipleNodesCanTakeDamage) {
    auto node1 = std::make_unique<MockNode>(400.0f, 300.0f, NodeShape::Circle, 30.0f, 100.0f);
    auto node2 = std::make_unique<MockNode>(450.0f, 300.0f, NodeShape::Square, 30.0f, 100.0f);
    std::vector<INode*> nodes = {node1.get(), node2.get()};

    int damageCount = 0;
    damageZoneService->ProcessDamageZone(425.0f, 300.0f, 100.0f, 50.0f, 1, nodes,
        [&](INode*, float) { damageCount++; });

    EXPECT_EQ(damageCount, 2);
    EXPECT_LT(node1->GetHP(), 100.0f);
    EXPECT_LT(node2->GetHP(), 100.0f);
}
