#include <gtest/gtest.h>

#include "../../NodeZero.Core/include/Game.h"
#include "../../NodeZero.Core/include/IGame.h"

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
    EXPECT_GE(game->GetCurrentLevel(), 1);
}

TEST_F(LevelSystemTest, ProgressBarStartsAtZero) {
    EXPECT_FLOAT_EQ(game->GetProgressBarPercentage(), 0.0f);
}

TEST_F(LevelSystemTest, ProgressBarIncreasesOverTime) {
    game->Update(1.0f);
    EXPECT_GT(game->GetProgressBarPercentage(), 0.0f);
}

TEST_F(LevelSystemTest, ProgressBarCapsAt100Percent) {
    for (int i = 0; i < 100; ++i) {
        game->Update(1.0f);
    }
    EXPECT_LE(game->GetProgressBarPercentage(), 100.0f);
}

TEST_F(LevelSystemTest, LevelCompletedAfterBossDefeat) {
    EXPECT_FALSE(game->IsLevelCompleted());
}
