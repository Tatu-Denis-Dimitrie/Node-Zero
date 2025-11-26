#include <gtest/gtest.h>

#include "../NodeZero.Core/src/Services/HealthService.h"
#include "../NodeZero.Core/src/Services/UpgradeService.h"
#include "../NodeZero.Core/src/Services/SaveService.h"
#include "../NodeZero.Core/include/Config/GameConfig.h"
#include "../NodeZero.Core/include/Types/SaveData.h"

// ============================================================================
// HealthService Tests
// ============================================================================
class HealthServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        healthService = std::make_unique<HealthService>();
        healthService->Initialize(100.0f, 5.0f);
    }
    std::unique_ptr<HealthService> healthService;
};

TEST_F(HealthServiceTest, InitializeAndBasicGetters) {
    EXPECT_FLOAT_EQ(healthService->GetMax(), 100.0f);
    EXPECT_FLOAT_EQ(healthService->GetCurrent(), 100.0f);
    EXPECT_FALSE(healthService->IsZero());
}

TEST_F(HealthServiceTest, DamageReducesHealth) {
    healthService->Reduce(30.0f);
    EXPECT_FLOAT_EQ(healthService->GetCurrent(), 70.0f);

    healthService->Reduce(150.0f);
    EXPECT_FLOAT_EQ(healthService->GetCurrent(), 0.0f);
    EXPECT_TRUE(healthService->IsZero());
}

TEST_F(HealthServiceTest, RegenerationWorks) {
    healthService->Reduce(50.0f);
    healthService->Update(2.0f);

    EXPECT_NEAR(healthService->GetCurrent(), 59.9f, 0.1f);
}

TEST_F(HealthServiceTest, DepletionScalesWithLevel) {
    float initialHealth = healthService->GetCurrent();

    healthService->SetCurrentLevel(1);
    healthService->Update(0.3f);
    float level1Health = healthService->GetCurrent();

    healthService->RestoreToMax();
    healthService->SetCurrentLevel(5);
    healthService->Update(0.3f);
    float level5Health = healthService->GetCurrent();

    EXPECT_LT(level5Health, level1Health);
}

TEST_F(HealthServiceTest, RestoreToMaxWorks) {
    healthService->Reduce(70.0f);
    healthService->RestoreToMax();
    EXPECT_FLOAT_EQ(healthService->GetCurrent(), 100.0f);
}

// ============================================================================
// UpgradeService Tests
// ============================================================================
class MockSaveService : public ISaveService {
private:
    SaveData m_Data;
public:
    MockSaveService() {
        m_Data.points = 1000;
        m_Data.maxHealth = GameConfig::HEALTH_DEFAULT;
        m_Data.regenRate = 0.0f;
        m_Data.damageZoneSize = GameConfig::DAMAGE_ZONE_DEFAULT_SIZE;
        m_Data.damagePerTick = GameConfig::DAMAGE_PER_TICK_DEFAULT;
    }

    SaveData LoadProgress() override { return m_Data; }
    void SaveProgress(const SaveData& data) override { m_Data = data; }
    void UpdatePoints(int points) override { m_Data.points = points; }
    void UpdateHighPoints(int highPoints) override { m_Data.highPoints = highPoints; }
    void UpdateGamesPlayed(int gamesPlayed) override { m_Data.gamesPlayed = gamesPlayed; }
    void UpdateTotalNodesDestroyed(int totalNodesDestroyed) override { m_Data.totalNodesDestroyed = totalNodesDestroyed; }
    void UpdateStats(float maxHealth, float regenRate, float damageZoneSize, float damagePerTick, int currentLevel) override {
        m_Data.maxHealth = maxHealth;
        m_Data.regenRate = regenRate;
        m_Data.damageZoneSize = damageZoneSize;
        m_Data.damagePerTick = damagePerTick;
        m_Data.currentLevel = currentLevel;
    }
    int GetPoints() const override { return m_Data.points; }
    int GetHighPoints() const override { return m_Data.highPoints; }
    SaveData GetCurrentData() const override { return m_Data; }
};

class UpgradeServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        upgradeService = std::make_unique<UpgradeService>();
        mockSaveService = std::make_unique<MockSaveService>();
        upgradeService->SetSaveService(mockSaveService.get());
        upgradeService->Initialize(GameConfig::HEALTH_DEFAULT, 0.0f, GameConfig::DAMAGE_ZONE_DEFAULT_SIZE, GameConfig::DAMAGE_PER_TICK_DEFAULT);
    }
    std::unique_ptr<UpgradeService> upgradeService;
    std::unique_ptr<MockSaveService> mockSaveService;
};

TEST_F(UpgradeServiceTest, BuyHealthUpgradeWorks) {
    int initialPoints = mockSaveService->GetPoints();
    float initialHealth = upgradeService->GetMaxHealth();

    EXPECT_TRUE(upgradeService->BuyHealthUpgrade());
    EXPECT_FLOAT_EQ(upgradeService->GetMaxHealth(), initialHealth + 1.0f);
    EXPECT_EQ(mockSaveService->GetPoints(), initialPoints - GameConfig::HEALTH_UPGRADE_COST);
}

TEST_F(UpgradeServiceTest, BuyRegenUpgradeWorks) {
    EXPECT_TRUE(upgradeService->BuyRegenUpgrade());
    EXPECT_FLOAT_EQ(upgradeService->GetRegenRate(), GameConfig::REGEN_UPGRADE_AMOUNT);
}

TEST_F(UpgradeServiceTest, BuyDamageZoneUpgradeWorks) {
    float initialSize = upgradeService->GetDamageZoneSize();
    EXPECT_TRUE(upgradeService->BuyDamageZoneUpgrade());
    EXPECT_FLOAT_EQ(upgradeService->GetDamageZoneSize(), initialSize + GameConfig::DAMAGE_ZONE_UPGRADE_AMOUNT);
}

TEST_F(UpgradeServiceTest, BuyDamageUpgradeWorks) {
    float initialDamage = upgradeService->GetDamagePerTick();
    EXPECT_TRUE(upgradeService->BuyDamageUpgrade());
    EXPECT_FLOAT_EQ(upgradeService->GetDamagePerTick(), initialDamage + GameConfig::DAMAGE_UPGRADE_AMOUNT);
}

TEST_F(UpgradeServiceTest, UpgradesFailWithInsufficientPoints) {
    SaveData data = mockSaveService->GetCurrentData();
    data.points = 10;
    mockSaveService->SaveProgress(data);

    EXPECT_FALSE(upgradeService->BuyHealthUpgrade());
    EXPECT_FALSE(upgradeService->BuyRegenUpgrade());
}

TEST_F(UpgradeServiceTest, DamageZoneSizeCannotExceedMaximum) {
    upgradeService->Initialize(GameConfig::HEALTH_DEFAULT, 0.0f, GameConfig::DAMAGE_ZONE_MAX_SIZE, GameConfig::DAMAGE_PER_TICK_DEFAULT);
    EXPECT_FALSE(upgradeService->BuyDamageZoneUpgrade());
}

// ============================================================================
// SaveService Tests
// ============================================================================
class SaveServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        saveService = std::make_unique<SaveService>();
    }
    std::unique_ptr<SaveService> saveService;
};

TEST_F(SaveServiceTest, SaveAndLoadData) {
    SaveData data;
    data.points = 300;
    data.highPoints = 800;
    data.gamesPlayed = 15;
    data.currentLevel = 7;

    saveService->SaveProgress(data);
    SaveData loadedData = saveService->GetCurrentData();

    EXPECT_EQ(loadedData.points, 300);
    EXPECT_EQ(loadedData.highPoints, 800);
    EXPECT_EQ(loadedData.gamesPlayed, 15);
    EXPECT_EQ(loadedData.currentLevel, 7);
}

TEST_F(SaveServiceTest, UpdatePointsWorks) {
    saveService->UpdatePoints(500);
    EXPECT_EQ(saveService->GetPoints(), 500);
}

TEST_F(SaveServiceTest, UpdateHighPointsWorks) {
    saveService->UpdateHighPoints(1000);
    EXPECT_EQ(saveService->GetHighPoints(), 1000);
}

TEST_F(SaveServiceTest, UpdateStatsWorks) {
    saveService->UpdateStats(75.0f, 5.0f, 150.0f, 80.0f, 10);

    SaveData data = saveService->GetCurrentData();
    EXPECT_FLOAT_EQ(data.maxHealth, 75.0f);
    EXPECT_FLOAT_EQ(data.regenRate, 5.0f);
    EXPECT_EQ(data.currentLevel, 10);
}
