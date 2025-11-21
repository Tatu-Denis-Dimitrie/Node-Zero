#pragma once

#include <vector>

#include "Events/Subject.h"
#include "IGame.h"
#include "INode.h"
#include "Types/PointPickup.h"

class Game : public IGame {
   private:
    std::vector<INode*> m_Nodes;
    std::vector<PointPickup> m_Pickups;
    float m_ScreenWidth;
    float m_ScreenHeight;
    Subject m_Subject;
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

    Subject& GetSubject() override;

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

   private:
    INode* CreateNode(NodeShape shape, float size, float speed);
    NodeShape GetRandomShape();
    void SpawnPointPickups(const Position& origin);
    void UpdatePickups(float deltaTime);
    float RandomRange(float minValue, float maxValue) const;

   private:
    static constexpr float PICKUP_LIFETIME = 10.0f;
    static constexpr float PICKUP_SIZE = 6.0f;
    static constexpr float PICKUP_COLLECT_DELAY = 1.0f;
    static constexpr int HEALTH_UPGRADE_COST = 50;
};
