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
    EXPECT_LE(game->GetHealthService().GetCurrent(), game->GetUpgradeService().GetMaxHealth());
}

TEST_F(HealthSystemTest, HealthDepletionOverTime) {
    float initialHealth = game->GetHealthService().GetCurrent();
    game->GetHealthService().Update(1.0f);
    EXPECT_LT(game->GetHealthService().GetCurrent(), initialHealth);
}

TEST_F(HealthSystemTest, DirectHealthReduction) {
    float initialHealth = game->GetHealthService().GetCurrent();
    game->GetHealthService().Reduce(0.5f);
    EXPECT_FLOAT_EQ(game->GetHealthService().GetCurrent(), initialHealth - 0.5f);

    game->GetHealthService().Reduce(0.5f);
    game->GetHealthService().Reduce(0.5f);

    EXPECT_FLOAT_EQ(game->GetHealthService().GetCurrent(), initialHealth - 1.5f);
}

TEST_F(HealthSystemTest, HealthCannotGoBelowZero) {
    float currentHealth = game->GetHealthService().GetCurrent();
    game->GetHealthService().Reduce(currentHealth + 10.0f);

    EXPECT_FLOAT_EQ(game->GetHealthService().GetCurrent(), 0.0f);
    EXPECT_TRUE(game->GetHealthService().IsZero());
}

TEST_F(HealthSystemTest, CombinedHealthReduction) {
    float initialHealth = game->GetHealthService().GetCurrent();
    game->GetHealthService().Reduce(2.0f);
    float healthAfterReduction = game->GetHealthService().GetCurrent();

    game->GetHealthService().Update(1.0f);

    EXPECT_LT(game->GetHealthService().GetCurrent(), initialHealth);
}

TEST_F(HealthSystemTest, HealthResetAfterGameOver) {
    float currentHealth = game->GetHealthService().GetCurrent();
    game->GetHealthService().Reduce(currentHealth + 10.0f);
    EXPECT_TRUE(game->GetHealthService().IsZero());

    game->Reset();
    EXPECT_FALSE(game->GetHealthService().IsZero());
    EXPECT_GT(game->GetHealthService().GetCurrent(), 0.0f);
}

TEST_F(HealthSystemTest, RealisticGameplayScenario) {
    float initialHealth = game->GetHealthService().GetCurrent();

    for (int i = 0; i < 10; ++i) {
        game->GetHealthService().Update(0.1f);
    }

    float healthAfterDepletion = game->GetHealthService().GetCurrent();
    EXPECT_LT(healthAfterDepletion, initialHealth);

    game->GetHealthService().Reduce(1.5f);
    EXPECT_LE(game->GetHealthService().GetCurrent(), healthAfterDepletion - 1.5f);
}
