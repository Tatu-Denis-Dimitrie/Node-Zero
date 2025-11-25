#include <gtest/gtest.h>

#include "../../NodeZero.Core/include/Game.h"
#include "../../NodeZero.Core/include/IGame.h"

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
    EXPECT_GT(game->GetUpgradeService().GetDamageZoneSize(), 0.0f);
}

TEST_F(UpgradeSystemTest, InitialDamagePerTick) {
    EXPECT_GT(game->GetUpgradeService().GetDamagePerTick(), 0.0f);
}

TEST_F(UpgradeSystemTest, HealthUpgradeCost) {
    EXPECT_EQ(game->GetUpgradeService().GetHealthUpgradeCost(), 50);
}

TEST_F(UpgradeSystemTest, DamageZoneUpgradeCost) {
    EXPECT_EQ(game->GetUpgradeService().GetDamageZoneUpgradeCost(), 75);
}

TEST_F(UpgradeSystemTest, DamageUpgradeCost) {
    EXPECT_EQ(game->GetUpgradeService().GetDamageUpgradeCost(), 60);
}

TEST_F(UpgradeSystemTest, RegenUpgradeCost) {
    EXPECT_EQ(game->GetUpgradeService().GetRegenUpgradeCost(), 100);
}

TEST_F(UpgradeSystemTest, InitialRegenRate) {
    EXPECT_GE(game->GetUpgradeService().GetRegenRate(), 0.0f);
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
    float initialMaxHealth = game->GetUpgradeService().GetMaxHealth();
    bool success = game->GetUpgradeService().BuyHealthUpgrade();
    if (success) {
        EXPECT_GT(game->GetUpgradeService().GetMaxHealth(), initialMaxHealth);
    }
}

TEST_F(UpgradePurchaseTest, RegenUpgradeIncreasesRegenRate) {
    float initialRegenRate = game->GetUpgradeService().GetRegenRate();
    bool success = game->GetUpgradeService().BuyRegenUpgrade();
    if (success) {
        EXPECT_GT(game->GetUpgradeService().GetRegenRate(), initialRegenRate);
    }
}

TEST_F(UpgradePurchaseTest, DamageZoneUpgradeIncreasesSize) {
    float initialSize = game->GetUpgradeService().GetDamageZoneSize();
    bool success = game->GetUpgradeService().BuyDamageZoneUpgrade();
    if (success) {
        EXPECT_GT(game->GetUpgradeService().GetDamageZoneSize(), initialSize);
    }
}

TEST_F(UpgradePurchaseTest, DamageUpgradeIncreasesDamage) {
    float initialDamage = game->GetUpgradeService().GetDamagePerTick();
    bool success = game->GetUpgradeService().BuyDamageUpgrade();
    if (success) {
        EXPECT_GT(game->GetUpgradeService().GetDamagePerTick(), initialDamage);
    }
}
