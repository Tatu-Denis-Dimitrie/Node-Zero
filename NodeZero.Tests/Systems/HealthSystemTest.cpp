#include <gtest/gtest.h>

#include "../../NodeZero.Core/include/Game.h"
#include "../../NodeZero.Core/include/IGame.h"

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
}

TEST_F(HealthSystemTest, HealthDepletionOverTime) {
    float initialHealth = game->GetCurrentHealth();
    game->UpdateHealth(1.0f);
    EXPECT_LT(game->GetCurrentHealth(), initialHealth);
}

TEST_F(HealthSystemTest, DirectHealthReduction) {
    float initialHealth = game->GetCurrentHealth();
    game->ReduceHealth(0.5f);
    EXPECT_FLOAT_EQ(game->GetCurrentHealth(), initialHealth - 0.5f);

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
    game->ReduceHealth(2.0f);
    game->UpdateHealth(1.0f);

    EXPECT_LT(game->GetCurrentHealth(), initialHealth - 2.0f);
}

TEST_F(HealthSystemTest, HealthResetAfterGameOver) {
    game->ReduceHealth(20.0f);
    EXPECT_TRUE(game->IsGameOver());

    game->Reset();
    EXPECT_FALSE(game->IsGameOver());
    EXPECT_GT(game->GetCurrentHealth(), 0.0f);
}

TEST_F(HealthSystemTest, RealisticGameplayScenario) {
    float initialHealth = game->GetCurrentHealth();

    for (int i = 0; i < 10; ++i) {
        game->UpdateHealth(0.1f);
    }

    float healthAfterDepletion = game->GetCurrentHealth();
    EXPECT_LT(healthAfterDepletion, initialHealth);

    game->ReduceHealth(1.5f);
    EXPECT_LE(game->GetCurrentHealth(), healthAfterDepletion - 1.5f);
}
