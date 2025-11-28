#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>

#include "../NodeZero.Core/src/Services/LevelService.h"
#include "../NodeZero.Core/src/Services/SpawnService.h"
#include "../NodeZero.Core/include/Config/GameConfig.h"
#include "../NodeZero.Core/include/Enums/NodeShape.h"
#include "../NodeZero.Core/include/Types/SpawnInfo.h"

class LevelServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        levelService = std::make_unique<LevelService>();
        levelService->Initialize(1);
    }
    std::unique_ptr<LevelService> levelService;
};

TEST_F(LevelServiceTest, InitializeAndGetters) {
    EXPECT_EQ(levelService->GetCurrentLevel(), 1);
    EXPECT_EQ(levelService->GetNodesDestroyedThisLevel(), 0);
    EXPECT_FALSE(levelService->IsBossActive());
    EXPECT_FALSE(levelService->IsLevelCompleted());
}

TEST_F(LevelServiceTest, StartNextLevelWorks) {
    levelService->StartNextLevel();

    EXPECT_EQ(levelService->GetCurrentLevel(), 2);
    EXPECT_EQ(levelService->GetNodesDestroyedThisLevel(), 0);
    EXPECT_FALSE(levelService->IsLevelCompleted());
}

TEST_F(LevelServiceTest, NodesDestroyedIncrement) {
    levelService->IncrementNodesDestroyed();
    levelService->IncrementNodesDestroyed();

    EXPECT_EQ(levelService->GetNodesDestroyedThisLevel(), 2);
}

TEST_F(LevelServiceTest, BossSpawningLogic) {
    EXPECT_FALSE(levelService->ShouldSpawnBoss());

    levelService->Update(GameConfig::LEVEL_DURATION);

    EXPECT_TRUE(levelService->ShouldSpawnBoss());

    levelService->SetBossActive(true);
    EXPECT_FALSE(levelService->ShouldSpawnBoss());
}

TEST_F(LevelServiceTest, ProgressBarWorks) {
    EXPECT_FLOAT_EQ(levelService->GetProgressBarPercentage(), 0.0f);

    levelService->Update(GameConfig::LEVEL_DURATION / 2.0f);
    EXPECT_FLOAT_EQ(levelService->GetProgressBarPercentage(), 50.0f);

    levelService->Update(GameConfig::LEVEL_DURATION);
    EXPECT_FLOAT_EQ(levelService->GetProgressBarPercentage(), 100.0f);
}

TEST_F(LevelServiceTest, ResetWorks) {
    levelService->StartNextLevel();
    levelService->IncrementNodesDestroyed();
    levelService->Update(30.0f);

    levelService->Reset(1);

    EXPECT_EQ(levelService->GetCurrentLevel(), 1);
    EXPECT_EQ(levelService->GetNodesDestroyedThisLevel(), 0);
    EXPECT_FLOAT_EQ(levelService->GetProgressBarPercentage(), 0.0f);
}

TEST_F(LevelServiceTest, TimerDoesNotAdvanceWhenBossIsActive) {
    levelService->Update(30.0f);
    float progress1 = levelService->GetProgressBarPercentage();

    levelService->SetBossActive(true);
    levelService->Update(10.0f, true);
    float progress2 = levelService->GetProgressBarPercentage();

    EXPECT_FLOAT_EQ(progress1, progress2);
}

class SpawnServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::srand(42);
        spawnService = std::make_unique<SpawnService>();
        spawnService->Initialize(800.0f, 600.0f);
    }
    std::unique_ptr<SpawnService> spawnService;
};

TEST_F(SpawnServiceTest, InitiallyNotReadyToSpawn) {
    EXPECT_FALSE(spawnService->ShouldAutoSpawn());
}

TEST_F(SpawnServiceTest, AutoSpawnWorks) {
    spawnService->UpdateAutoSpawn(1.0f);
    EXPECT_FALSE(spawnService->ShouldAutoSpawn());

    spawnService->UpdateAutoSpawn(1.1f);
    EXPECT_TRUE(spawnService->ShouldAutoSpawn());
}

TEST_F(SpawnServiceTest, HigherLevelsSpawnFaster) {
    spawnService->SetCurrentLevel(1);
    spawnService->UpdateAutoSpawn(1.5f);
    bool shouldSpawnLevel1 = spawnService->ShouldAutoSpawn();

    spawnService->ResetSpawnTimer();

    spawnService->SetCurrentLevel(5);
    spawnService->UpdateAutoSpawn(1.5f);
    bool shouldSpawnLevel5 = spawnService->ShouldAutoSpawn();

    EXPECT_FALSE(shouldSpawnLevel1);
    EXPECT_TRUE(shouldSpawnLevel5);
}

TEST_F(SpawnServiceTest, GetRandomShapeReturnsValidShape) {
    for (int i = 0; i < 50; ++i) {
        SpawnInfo info = spawnService->GetNextSpawn();
        EXPECT_TRUE(info.shape == NodeShape::Circle ||
                   info.shape == NodeShape::Square ||
                   info.shape == NodeShape::Hexagon);
    }
}

TEST_F(SpawnServiceTest, HPScalesWithLevel) {
    spawnService->SetCurrentLevel(1);
    float hp1 = spawnService->CalculateNodeHP(100.0f);

    spawnService->SetCurrentLevel(5);
    float hp5 = spawnService->CalculateNodeHP(100.0f);

    EXPECT_GT(hp5, hp1);
    EXPECT_FLOAT_EQ(hp5, 100.0f * (1.0f + 4 * 0.2f));
}

TEST_F(SpawnServiceTest, ResetClearsTimer) {
    spawnService->UpdateAutoSpawn(1.9f);
    spawnService->ResetSpawnTimer();

    EXPECT_FALSE(spawnService->ShouldAutoSpawn());
}

TEST_F(SpawnServiceTest, GetNextSpawnReturnsValidData) {
    SpawnInfo info = spawnService->GetNextSpawn();

    EXPECT_TRUE(info.shape == NodeShape::Circle ||
               info.shape == NodeShape::Square ||
               info.shape == NodeShape::Hexagon);

    float length = std::sqrt(info.directionX * info.directionX + info.directionY * info.directionY);
    EXPECT_NEAR(length, 1.0f, 0.001f);
}
