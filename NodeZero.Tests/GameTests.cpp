#include <gtest/gtest.h>

#include "../NodeZero.Core/include/Game.h"
#include "../NodeZero.Core/include/IGame.h"

TEST(GameTest, PlaceholderTest) {
    EXPECT_TRUE(true);
}

class HealthSystemTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(HealthSystemTest, InitialHealthValues) {
    EXPECT_LE(game->GetCurrentHealth(), game->GetMaxHealth());
    EXPECT_GT(game->GetMaxHealth(), 0.0f);
    EXPECT_FALSE(game->IsGameOver());
}

TEST_F(HealthSystemTest, HealthDepletionOverTime) {
    float initialHealth = game->GetCurrentHealth();
    game->UpdateHealth(0.3f);
    game->UpdateHealth(0.3f);
    game->UpdateHealth(0.3f);

    EXPECT_LT(game->GetCurrentHealth(), initialHealth);
}

TEST_F(HealthSystemTest, DirectHealthReduction) {
    float initialHealth = game->GetCurrentHealth();
    game->ReduceHealth(0.5f);
    game->ReduceHealth(0.5f);
    game->ReduceHealth(0.5f);

    EXPECT_FLOAT_EQ(game->GetCurrentHealth(), initialHealth - 1.5f);
}

TEST_F(HealthSystemTest, HealthCannotGoBelowZero) {
    game->ReduceHealth(20.0f);

    EXPECT_FLOAT_EQ(game->GetCurrentHealth(), 0.0f);
    EXPECT_TRUE(game->IsGameOver());
}

TEST_F(HealthSystemTest, CombinedHealthReduction) {
    float initialHealth = game->GetCurrentHealth();
    game->UpdateHealth(0.3f);

    game->ReduceHealth(0.5f);

    game->UpdateHealth(0.3f);

    EXPECT_LT(game->GetCurrentHealth(), initialHealth);
}

TEST_F(HealthSystemTest, HealthResetAfterGameOver) {
    game->ReduceHealth(1000.0f);
    EXPECT_TRUE(game->IsGameOver());

    game->Reset();

    EXPECT_EQ(game->GetCurrentHealth(), game->GetMaxHealth());
    EXPECT_FALSE(game->IsGameOver());
}

TEST_F(HealthSystemTest, RealisticGameplayScenario) {
    float initialHealth = game->GetCurrentHealth();
    for (int i = 0; i < 16; ++i) {
        game->UpdateHealth(0.3f);
    }

    game->ReduceHealth(0.5f);
    game->ReduceHealth(0.5f);
    game->ReduceHealth(0.5f);

    EXPECT_LT(game->GetCurrentHealth(), initialHealth - 1.0f);
    EXPECT_GT(game->GetCurrentHealth(), 0.0f);
}

class LevelSystemTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(LevelSystemTest, InitialLevelIsOne) {
    EXPECT_GT(game->GetCurrentLevel(), 0);
}

TEST_F(LevelSystemTest, ProgressBarStartsAtZero) {
    EXPECT_FLOAT_EQ(game->GetProgressBarPercentage(), 0.0f);
}

TEST_F(LevelSystemTest, ProgressBarIncreasesOverTime) {
    game->Update(30.0f);
    float progress = game->GetProgressBarPercentage();
    EXPECT_GT(progress, 0.0f);
    EXPECT_LT(progress, 100.0f);
}

TEST_F(LevelSystemTest, ProgressBarCapsAt100Percent) {
    game->Update(120.0f);
    EXPECT_FLOAT_EQ(game->GetProgressBarPercentage(), 100.0f);
}

TEST_F(LevelSystemTest, LevelCompletedAfterBossDefeat) {
    game->Update(61.0f);
    EXPECT_FALSE(game->IsLevelCompleted());
}

class UpgradeSystemTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(UpgradeSystemTest, InitialDamageZoneSize) {
    EXPECT_GT(game->GetDamageZoneSize(), 0.0f);
}

TEST_F(UpgradeSystemTest, InitialDamagePerTick) {
    EXPECT_GT(game->GetDamagePerTick(), 0.0f);
}

TEST_F(UpgradeSystemTest, HealthUpgradeCost) {
    EXPECT_EQ(game->GetHealthUpgradeCost(), 50);
}

TEST_F(UpgradeSystemTest, DamageZoneUpgradeCost) {
    EXPECT_EQ(game->GetDamageZoneUpgradeCost(), 75);
}

TEST_F(UpgradeSystemTest, DamageUpgradeCost) {
    EXPECT_EQ(game->GetDamageUpgradeCost(), 60);
}

TEST_F(UpgradeSystemTest, RegenUpgradeCost) {
    EXPECT_EQ(game->GetRegenUpgradeCost(), 100);
}

TEST_F(UpgradeSystemTest, InitialRegenRate) {
    EXPECT_GE(game->GetRegenRate(), 0.0f);
}

class NodeSpawnTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(NodeSpawnTest, NoNodesAtStart) {
    const auto& nodes = game->GetNodes();
    EXPECT_EQ(nodes.size(), 0);
}

TEST_F(NodeSpawnTest, NodeSpawnsAtPosition) {
    game->SpawnNode(400.0f, 300.0f);
    const auto& nodes = game->GetNodes();
    EXPECT_EQ(nodes.size(), 1);
}

TEST_F(NodeSpawnTest, MultipleNodesCanSpawn) {
    game->SpawnNode(400.0f, 300.0f);
    game->SpawnNode(200.0f, 150.0f);
    game->SpawnNode(600.0f, 450.0f);
    const auto& nodes = game->GetNodes();
    EXPECT_EQ(nodes.size(), 3);
}

class PickupSystemTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(PickupSystemTest, InitialScoreIsZero) {
    EXPECT_EQ(game->GetPickupScore(), 0);
}

TEST_F(PickupSystemTest, NoPickupsAtStart) {
    const auto& pickups = game->GetPickups();
    EXPECT_EQ(pickups.size(), 0);
}

TEST_F(PickupSystemTest, NodesDestroyedCountStartsAtZero) {
    EXPECT_EQ(game->GetNodesDestroyed(), 0);
}

class DamageSystemTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(DamageSystemTest, InitialDamageTimerState) {
    EXPECT_FALSE(game->ShouldDealDamage());
}

TEST_F(DamageSystemTest, DamageTimerProgresses) {
    game->UpdateDamageTimer(1.0f);
    game->UpdateDamageTimer(0.6f);
    EXPECT_TRUE(game->ShouldDealDamage());
}

TEST_F(DamageSystemTest, DamageTimerResets) {
    game->UpdateDamageTimer(2.0f);
    game->ResetDamageTimer();
    EXPECT_FALSE(game->ShouldDealDamage());
}
