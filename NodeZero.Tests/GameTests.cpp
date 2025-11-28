#include <gtest/gtest.h>

#include <memory>

#include "../NodeZero.Core/src/Game.h"
#include "../NodeZero.Core/include/IGame.h"
#include "../NodeZero.Core/include/INode.h"
#include "../NodeZero.Core/include/Events/IObserver.h"
#include "../NodeZero.Core/include/Events/IEvent.h"
#include "../NodeZero.Core/include/Types/SpawnInfo.h"

// Helper function to create SpawnInfo for tests
static SpawnInfo CreateTestSpawnInfo(float x, float y) {
    SpawnInfo info;
    info.position = Position{x, y};
    info.shape = NodeShape::Circle;
    info.directionX = 0.0f;
    info.directionY = 0.0f;
    return info;
}

class MockObserver : public IObserver {
private:
    int m_EventCount = 0;
public:
    void Update(const std::shared_ptr<IEvent>& event) override {
        m_EventCount++;
    }
    int GetEventCount() const { return m_EventCount; }
};

class GameTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
        game->Initialize(800.0f, 600.0f);
        game->Reset();
    }
    std::unique_ptr<IGame> game;
};

TEST_F(GameTest, InitializeWorks) {
    EXPECT_FLOAT_EQ(game->GetScreenWidth(), 800.0f);
    EXPECT_FLOAT_EQ(game->GetScreenHeight(), 600.0f);
    EXPECT_EQ(game->GetNodes().size(), 0);
    EXPECT_FALSE(game->GetHealthService().IsZero());
}

TEST_F(GameTest, SpawnNodeWorks) {
    game->SpawnNode(CreateTestSpawnInfo(400.0f, 300.0f));

    const auto& nodes = game->GetNodes();
    EXPECT_EQ(nodes.size(), 1);
    EXPECT_FLOAT_EQ(nodes[0]->GetPosition().x, 400.0f);
    EXPECT_FLOAT_EQ(nodes[0]->GetPosition().y, 300.0f);
}

TEST_F(GameTest, MultipleNodesCanSpawn) {
    game->SpawnNode(CreateTestSpawnInfo(100.0f, 100.0f));
    game->SpawnNode(CreateTestSpawnInfo(200.0f, 200.0f));
    game->SpawnNode(CreateTestSpawnInfo(300.0f, 300.0f));

    EXPECT_EQ(game->GetNodes().size(), 3);
}

TEST_F(GameTest, UpdateDoesNotCrash) {
    EXPECT_NO_THROW(game->Update(0.016f));
    EXPECT_NO_THROW(game->Update(1.0f));
}

TEST_F(GameTest, UpdateAdvancesLevelProgress) {
    float progressBefore = game->GetLevelService().GetProgressBarPercentage();
    game->Update(10.0f);
    float progressAfter = game->GetLevelService().GetProgressBarPercentage();

    EXPECT_GT(progressAfter, progressBefore);
}

TEST_F(GameTest, UpdateSpawnsNodesOverTime) {
    size_t initialNodeCount = game->GetNodes().size();

    for (int i = 0; i < 50; ++i) {
        game->Update(0.1f);
    }

    EXPECT_GT(game->GetNodes().size(), initialNodeCount);
}

TEST_F(GameTest, ResetClearsNodes) {
    game->SpawnNode(CreateTestSpawnInfo(100.0f, 100.0f));
    game->SpawnNode(CreateTestSpawnInfo(200.0f, 200.0f));
    game->Update(10.0f);

    game->Reset();

    EXPECT_EQ(game->GetNodes().size(), 0);
    EXPECT_EQ(game->GetNodesDestroyed(), 0);
}

TEST_F(GameTest, StartNextLevelWorks) {
    int initialLevel = game->GetLevelService().GetCurrentLevel();

    game->StartNextLevel();

    EXPECT_EQ(game->GetLevelService().GetCurrentLevel(), initialLevel + 1);
    EXPECT_EQ(game->GetNodes().size(), 0);
}

TEST_F(GameTest, SetMousePositionWorks) {
    EXPECT_NO_THROW(game->SetMousePosition(400.0f, 300.0f));
}

TEST_F(GameTest, SaveProgressWorks) {
    EXPECT_NO_THROW(game->SaveProgress());
}

TEST_F(GameTest, AllServicesAreAccessible) {
    EXPECT_NO_THROW(game->GetHealthService());
    EXPECT_NO_THROW(game->GetLevelService());
    EXPECT_NO_THROW(game->GetUpgradeService());
    EXPECT_NO_THROW(game->GetPickupService());
    EXPECT_NO_THROW(game->GetDamageZoneService());
    EXPECT_NO_THROW(game->GetSpawnService());
    EXPECT_NO_THROW(game->GetSaveService());
}

TEST_F(GameTest, HealthServiceIntegration) {
    auto& healthService = game->GetHealthService();
    EXPECT_GT(healthService.GetMax(), 0.0f);
    EXPECT_GT(healthService.GetCurrent(), 0.0f);
}

TEST_F(GameTest, LevelServiceIntegration) {
    auto& levelService = game->GetLevelService();
    EXPECT_GE(levelService.GetCurrentLevel(), 1);
    EXPECT_EQ(levelService.GetNodesDestroyedThisLevel(), 0);
}

TEST_F(GameTest, SaveServiceIntegration) {
    auto& saveService = game->GetSaveService();
    EXPECT_GE(saveService.GetPoints(), 0);
    EXPECT_GE(saveService.GetHighPoints(), 0);
}

TEST_F(GameTest, ObserverPatternWorks) {
    auto observer = std::make_shared<MockObserver>();

    game->Attach(observer);
    EXPECT_EQ(observer->GetEventCount(), 0);

    for (int i = 0; i < 50; ++i) {
        game->Update(0.1f);
    }

    EXPECT_GT(observer->GetEventCount(), 0);

    game->Detach(observer);
}

TEST_F(GameTest, CompleteGameCycleWorks) {
    int initialLevel = game->GetLevelService().GetCurrentLevel();

    for (int i = 0; i < 100; ++i) {
        game->SetMousePosition(400.0f, 300.0f);
        game->Update(0.016f);
    }

    game->StartNextLevel();
    EXPECT_EQ(game->GetLevelService().GetCurrentLevel(), initialLevel + 1);

    game->Reset();
    EXPECT_EQ(game->GetNodes().size(), 0);
}

TEST_F(GameTest, MultiLevelProgression) {
    int startLevel = game->GetLevelService().GetCurrentLevel();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 50; ++j) {
            game->Update(0.1f);
        }

        if (i < 2) {
            game->StartNextLevel();
        }
    }

    EXPECT_EQ(game->GetLevelService().GetCurrentLevel(), startLevel + 2);
}

TEST_F(GameTest, ManyNodesDoNotCrash) {
    for (int i = 0; i < 50; ++i) {
        game->SpawnNode(CreateTestSpawnInfo(400.0f + i * 10, 300.0f));
    }

    EXPECT_NO_THROW(game->Update(0.016f));
}

TEST_F(GameTest, LongRunningGameWorks) {
    for (int i = 0; i < 500; ++i) {
        game->Update(0.016f);
    }

    EXPECT_NO_THROW(game->SaveProgress());
}

TEST_F(GameTest, MultipleResetsCycleCorrectly) {
    for (int i = 0; i < 5; ++i) {
        game->SpawnNode(CreateTestSpawnInfo(100.0f, 100.0f));
        game->Update(1.0f);
        game->Reset();

        EXPECT_EQ(game->GetNodes().size(), 0);
    }
}
