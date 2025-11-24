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
