#pragma once

#include <list>
#include <memory>
#include <vector>

#include "Events/IObserver.h"
#include "IGame.h"
#include "INode.h"
#include "Types/PointPickup.h"

class Game : public IGame {
   private:
    std::vector<INode*> m_Nodes;
    std::vector<PointPickup> m_Pickups;
    std::list<std::shared_ptr<IObserver>> m_observers;
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_ElapsedTime;
    int m_NextPickupId;
    int m_PickupScore;

    float m_MaxHealth;
    float m_CurrentHealth;
    float m_HealthDepletionRate;
    float m_HealthDepletionInterval;
    float m_HealthTimer;

    int m_NodesDestroyed;
    int m_HighScore;

    float m_SpawnTimer;
    float m_SpawnInterval;

    float m_DamageTimer;
    float m_DamageInterval;

    float m_DamageZoneSize;
    float m_DamagePerTick;

    float m_ProgressBarPercentage;

    int m_CurrentLevel;
    int m_NodesDestroyedThisLevel;
    bool m_BossActive;
    INode* m_Boss;
    float m_LevelTimer;
    float m_LevelDuration;

   public:
    Game();
    ~Game();

    void Initialize(float screenWidth, float screenHeight) override;
    void Update(float deltaTime) override;

    float GetScreenWidth() const override;
    float GetScreenHeight() const override;

    const std::vector<INode*>& GetNodes() const override;
    const std::vector<PointPickup>& GetPickups() const override;

    void SpawnNode(float x, float y) override;
    void UpdateAutoSpawn(float deltaTime) override;
    bool CollectPickup(int pickupId) override;
    void ProcessDamageZone(float centerX, float centerY, float zoneSize, float damage, bool shouldDealDamage) override;
    void ProcessPickupCollection(float centerX, float centerY, float zoneSize) override;
    void UpdateDamageTimer(float deltaTime) override;
    bool ShouldDealDamage() const override;
    void ResetDamageTimer() override;

    void Reset() override;
    bool ShouldGameOver() const override;

    int GetPickupScore() const override;

    float GetCurrentHealth() const override;
    float GetMaxHealth() const override;
    void ReduceHealth(float amount) override;
    void UpdateHealth(float deltaTime) override;
    bool IsGameOver() const override;

    int GetNodesDestroyed() const override;
    void SaveProgress() override;
    int GetHighScore() const override;

    bool BuyHealthUpgrade() override;
    int GetHealthUpgradeCost() const override;

    float GetDamageZoneSize() const override;
    bool BuyDamageZoneUpgrade() override;
    int GetDamageZoneUpgradeCost() const override;

    float GetDamagePerTick() const override;
    bool BuyDamageUpgrade() override;
    int GetDamageUpgradeCost() const override;

    float GetProgressBarPercentage() const override;

    int GetCurrentLevel() const override;
    int GetNodesDestroyedThisLevel() const override;
    bool IsBossActive() const override;

    // ISubject methods
    void Attach(std::shared_ptr<IObserver> observer) override;
    void Detach(std::shared_ptr<IObserver> observer) override;
    void Notify(const std::shared_ptr<IEvent>& event) override;

   private:
    INode* CreateNode(NodeShape shape, float size, float speed);
    NodeShape GetRandomShape();
    void SpawnPointPickups(const Position& origin);
    void UpdatePickups(float deltaTime);
    float RandomRange(float minValue, float maxValue) const;
    void SpawnBoss();
    void CheckBossDefeat();
    void AdvanceToNextLevel();

   private:
    static constexpr float PICKUP_LIFETIME = 10.0f;
    static constexpr float PICKUP_SIZE = 6.0f;
    static constexpr float PICKUP_COLLECT_DELAY = 1.0f;
    static constexpr int HEALTH_UPGRADE_COST = 50;
    static constexpr int DAMAGE_ZONE_UPGRADE_COST = 75;
    static constexpr float DAMAGE_ZONE_UPGRADE_AMOUNT = 10.0f;
    static constexpr int DAMAGE_UPGRADE_COST = 60;
    static constexpr float DAMAGE_UPGRADE_AMOUNT = 5.0f;

    static constexpr int NODES_PER_LEVEL = 20;
    static constexpr float BOSS_SIZE = 120.0f;
    static constexpr float BOSS_SPEED = 25.0f;
    static constexpr float BOSS_HP_BASE = 200.0f;
    static constexpr float LEVEL_DURATION = 60.0f;  // 60 seconds per level
};
