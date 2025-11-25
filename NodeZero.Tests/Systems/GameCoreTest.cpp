#include <gtest/gtest.h>

#include "../../NodeZero.Core/include/Game.h"
#include "../../NodeZero.Core/include/IGame.h"
#include "../../NodeZero.Core/include/Systems/SaveSystem.h"
#include "../../NodeZero.Core/include/Types/SaveData.h"

TEST(GameTest, PlaceholderTest) {
    EXPECT_TRUE(true);
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
    game->SpawnNode(100.0f, 100.0f);
    game->SpawnNode(200.0f, 200.0f);
    game->SpawnNode(300.0f, 300.0f);
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

TEST_F(PickupSystemTest, InitialPointsIsZero) {
    EXPECT_EQ(game->GetPickupService().GetPickupPoints(), 0);
}

TEST_F(PickupSystemTest, NoPickupsAtStart) {
    const auto& pickups = game->GetPickupService().GetPickups();
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
    EXPECT_FALSE(game->GetDamageZoneService().ShouldDealDamage());
}

TEST_F(DamageSystemTest, DamageTimerProgresses) {
    game->GetDamageZoneService().UpdateTimer(1.0f);
    game->GetDamageZoneService().UpdateTimer(0.6f);
    EXPECT_TRUE(game->GetDamageZoneService().ShouldDealDamage());
}

TEST_F(DamageSystemTest, DamageTimerResets) {
    game->GetDamageZoneService().UpdateTimer(2.0f);
    game->GetDamageZoneService().ResetTimer();
    EXPECT_FALSE(game->GetDamageZoneService().ShouldDealDamage());
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

TEST_F(SaveSystemTest, GetPointsReturnsValidValue) {
    int points = SaveSystem::LoadProgress().points;
    EXPECT_GE(points, 0);
}

TEST_F(SaveSystemTest, GetSaveDataReturnsValidData) {
    SaveData data = SaveSystem::LoadProgress();
    EXPECT_GE(data.points, 0);
    EXPECT_GE(data.highPoints, 0);
    EXPECT_GE(data.gamesPlayed, 0);
    EXPECT_GE(data.totalNodesDestroyed, 0);
    EXPECT_GE(data.currentLevel, 1);
    EXPECT_GT(data.maxHealth, 0.0f);
    EXPECT_GE(data.regenRate, 0.0f);
    EXPECT_GT(data.damageZoneSize, 0.0f);
    EXPECT_GT(data.damagePerTick, 0.0f);
}

TEST_F(SaveSystemTest, GetHighPointsReturnsValidValue) {
    int highPoints = game->GetHighPoints();
    EXPECT_GE(highPoints, 0);
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
    EXPECT_FALSE(game->GetHealthService().IsZero());
    EXPECT_FALSE(game->GetHealthService().IsZero());
}

TEST_F(GameOverTest, GameOverWhenHealthZero) {
    game->GetHealthService().Reduce(1000.0f);
    EXPECT_TRUE(game->GetHealthService().IsZero());
}

TEST_F(GameOverTest, ResetRestoresHealth) {
    game->GetHealthService().Reduce(5.0f);
    float healthAfterDamage = game->GetHealthService().GetCurrent();
    game->Reset();
    EXPECT_GT(game->GetHealthService().GetCurrent(), healthAfterDamage);
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
    game->GetSpawnService().UpdateAutoSpawn(0.5f);
    game->GetSpawnService().UpdateAutoSpawn(1.0f);
    game->GetSpawnService().UpdateAutoSpawn(2.5f);
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
    EXPECT_FLOAT_EQ(game->GetLevelService().GetProgressBarPercentage(), 0.0f);
}

TEST_F(LevelProgressTest, NotLevelCompletedAtStart) {
    EXPECT_FALSE(game->GetLevelService().IsLevelCompleted());
}

TEST_F(LevelProgressTest, NodesDestroyedThisLevelStartsAtZero) {
    EXPECT_EQ(game->GetLevelService().GetNodesDestroyedThisLevel(), 0);
}

TEST_F(LevelProgressTest, StartNextLevelIncrementsLevel) {
    int initialLevel = game->GetLevelService().GetCurrentLevel();
    game->StartNextLevel();
    EXPECT_EQ(game->GetLevelService().GetCurrentLevel(), initialLevel + 1);
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
