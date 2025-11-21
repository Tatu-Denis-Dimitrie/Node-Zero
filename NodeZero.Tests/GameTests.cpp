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
    EXPECT_EQ(game->GetMaxHealth(), 15.0f);
    EXPECT_EQ(game->GetCurrentHealth(), 15.0f);
    EXPECT_FALSE(game->IsGameOver());
}

TEST_F(HealthSystemTest, HealthDepletionOverTime) {
    game->UpdateHealth(0.3f);
    game->UpdateHealth(0.3f);
    game->UpdateHealth(0.3f);

    EXPECT_FLOAT_EQ(game->GetCurrentHealth(), 14.7f);
}

TEST_F(HealthSystemTest, DirectHealthReduction) {
    game->ReduceHealth(0.5f);
    game->ReduceHealth(0.5f);
    game->ReduceHealth(0.5f);

    EXPECT_FLOAT_EQ(game->GetCurrentHealth(), 13.5f);
}

TEST_F(HealthSystemTest, HealthCannotGoBelowZero) {
    game->ReduceHealth(20.0f);

    EXPECT_FLOAT_EQ(game->GetCurrentHealth(), 0.0f);
    EXPECT_TRUE(game->IsGameOver());
}

TEST_F(HealthSystemTest, CombinedHealthReduction) {
    game->UpdateHealth(0.3f);

    game->ReduceHealth(0.5f);

    game->UpdateHealth(0.3f);

    EXPECT_FLOAT_EQ(game->GetCurrentHealth(), 14.3f);
}

TEST_F(HealthSystemTest, HealthResetAfterGameOver) {
    game->ReduceHealth(15.0f);
    EXPECT_TRUE(game->IsGameOver());

    game->Reset();

    EXPECT_FLOAT_EQ(game->GetCurrentHealth(), 15.0f);
    EXPECT_FALSE(game->IsGameOver());
}

TEST_F(HealthSystemTest, RealisticGameplayScenario) {
    for (int i = 0; i < 16; ++i) {
        game->UpdateHealth(0.3f);
    }

    game->ReduceHealth(0.5f);
    game->ReduceHealth(0.5f);
    game->ReduceHealth(0.5f);

    EXPECT_NEAR(game->GetCurrentHealth(), 11.9f, 0.001f);
    EXPECT_FALSE(game->IsGameOver());
}
