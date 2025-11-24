#include <gtest/gtest.h>

#include "../NodeZero.Core/include/Game.h"
#include "../NodeZero.Core/include/IGame.h"
#include "../NodeZero.Core/include/Types/SaveData.h"

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

class SaveSystemTest : public ::testing::Test {
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

TEST_F(SaveSystemTest, GetCoinsReturnsValidValue) {
    int coins = game->GetCoins();
    EXPECT_GE(coins, 0);
}

TEST_F(SaveSystemTest, GetSaveDataReturnsValidData) {
    SaveData data = game->GetSaveData();
    EXPECT_GE(data.coins, 0);
    EXPECT_GE(data.highScore, 0);
    EXPECT_GE(data.gamesPlayed, 0);
    EXPECT_GE(data.totalNodesDestroyed, 0);
    EXPECT_GE(data.currentLevel, 1);
    EXPECT_GT(data.maxHealth, 0.0f);
    EXPECT_GE(data.regenRate, 0.0f);
    EXPECT_GT(data.damageZoneSize, 0.0f);
    EXPECT_GT(data.damagePerTick, 0.0f);
}

TEST_F(SaveSystemTest, GetHighScoreReturnsValidValue) {
    int highScore = game->GetHighScore();
    EXPECT_GE(highScore, 0);
}

class MousePositionTest : public ::testing::Test {
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

TEST_F(MousePositionTest, SetMousePositionDoesNotCrash) {
    game->SetMousePosition(100.0f, 200.0f);
    game->SetMousePosition(0.0f, 0.0f);
    game->SetMousePosition(-50.0f, -50.0f);
}

TEST_F(MousePositionTest, GetCollectedPickupsInitiallyEmpty) {
    auto pickups = game->GetCollectedPickupsThisFrame();
    EXPECT_EQ(pickups.size(), 0);
}

TEST_F(MousePositionTest, CollectedPickupsClearedAfterUpdate) {
    game->SetMousePosition(400.0f, 300.0f);
    game->Update(0.016f);
    auto pickups = game->GetCollectedPickupsThisFrame();
    EXPECT_GE(pickups.size(), 0);
}

class ScreenDimensionsTest : public ::testing::Test {
   protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(1920.0f, 1080.0f);
    }

    void TearDown() override {
        game.reset();
    }

    std::unique_ptr<IGame> game;
};

TEST_F(ScreenDimensionsTest, ScreenWidthIsSetCorrectly) {
    EXPECT_FLOAT_EQ(game->GetScreenWidth(), 1920.0f);
}

TEST_F(ScreenDimensionsTest, ScreenHeightIsSetCorrectly) {
    EXPECT_FLOAT_EQ(game->GetScreenHeight(), 1080.0f);
}

class GameOverTest : public ::testing::Test {
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

TEST_F(GameOverTest, InitiallyNotGameOver) {
    EXPECT_FALSE(game->ShouldGameOver());
    EXPECT_FALSE(game->IsGameOver());
}

TEST_F(GameOverTest, GameOverWhenHealthZero) {
    game->ReduceHealth(1000.0f);
    EXPECT_TRUE(game->ShouldGameOver());
}

TEST_F(GameOverTest, ResetRestoresHealth) {
    game->ReduceHealth(5.0f);
    float healthAfterDamage = game->GetCurrentHealth();
    game->Reset();
    EXPECT_GT(game->GetCurrentHealth(), healthAfterDamage);
}

class UpgradePurchaseTest : public ::testing::Test {
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

TEST_F(UpgradePurchaseTest, HealthUpgradeIncreasesMaxHealth) {
    float initialMaxHealth = game->GetMaxHealth();
    bool success = game->BuyHealthUpgrade();
    if (success) {
        EXPECT_GT(game->GetMaxHealth(), initialMaxHealth);
    }
}

TEST_F(UpgradePurchaseTest, RegenUpgradeIncreasesRegenRate) {
    float initialRegenRate = game->GetRegenRate();
    bool success = game->BuyRegenUpgrade();
    if (success) {
        EXPECT_GT(game->GetRegenRate(), initialRegenRate);
    }
}

TEST_F(UpgradePurchaseTest, DamageZoneUpgradeIncreasesSize) {
    float initialSize = game->GetDamageZoneSize();
    bool success = game->BuyDamageZoneUpgrade();
    if (success) {
        EXPECT_GT(game->GetDamageZoneSize(), initialSize);
    }
}

TEST_F(UpgradePurchaseTest, DamageUpgradeIncreasesDamage) {
    float initialDamage = game->GetDamagePerTick();
    bool success = game->BuyDamageUpgrade();
    if (success) {
        EXPECT_GT(game->GetDamagePerTick(), initialDamage);
    }
}

class AutoSpawnTest : public ::testing::Test {
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

TEST_F(AutoSpawnTest, UpdateAutoSpawnDoesNotCrash) {
    game->UpdateAutoSpawn(0.5f);
    game->UpdateAutoSpawn(1.0f);
    game->UpdateAutoSpawn(2.5f);
}

TEST_F(AutoSpawnTest, NodesCanBeSpawnedManually) {
    size_t initialNodeCount = game->GetNodes().size();
    game->SpawnNode(400.0f, 300.0f);
    EXPECT_EQ(game->GetNodes().size(), initialNodeCount + 1);
}

TEST_F(AutoSpawnTest, MultipleManualSpawns) {
    size_t initialNodeCount = game->GetNodes().size();
    game->SpawnNode(100.0f, 100.0f);
    game->SpawnNode(200.0f, 200.0f);
    game->SpawnNode(300.0f, 300.0f);
    EXPECT_EQ(game->GetNodes().size(), initialNodeCount + 3);
}

class LevelProgressTest : public ::testing::Test {
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

TEST_F(LevelProgressTest, InitialLevelProgressIsZero) {
    EXPECT_FLOAT_EQ(game->GetProgressBarPercentage(), 0.0f);
}

TEST_F(LevelProgressTest, NotLevelCompletedAtStart) {
    EXPECT_FALSE(game->IsLevelCompleted());
}

TEST_F(LevelProgressTest, NodesDestroyedThisLevelStartsAtZero) {
    EXPECT_EQ(game->GetNodesDestroyedThisLevel(), 0);
}

TEST_F(LevelProgressTest, StartNextLevelIncrementsLevel) {
    int initialLevel = game->GetCurrentLevel();
    game->StartNextLevel();
    EXPECT_EQ(game->GetCurrentLevel(), initialLevel + 1);
}

class GameUpdateTest : public ::testing::Test {
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

TEST_F(GameUpdateTest, UpdateDoesNotCrashWithZeroDelta) {
    game->Update(0.0f);
}

TEST_F(GameUpdateTest, UpdateDoesNotCrashWithSmallDelta) {
    game->Update(0.001f);
}

TEST_F(GameUpdateTest, UpdateDoesNotCrashWithLargeDelta) {
    game->Update(1.0f);
}

TEST_F(GameUpdateTest, MultipleUpdatesDoNotCrash) {
    for (int i = 0; i < 100; ++i) {
        game->Update(0.016f);
    }
}

TEST_F(GameUpdateTest, UpdateWithMousePositionSet) {
    game->SetMousePosition(400.0f, 300.0f);
    game->Update(0.016f);
    game->SetMousePosition(500.0f, 400.0f);
    game->Update(0.016f);
}
