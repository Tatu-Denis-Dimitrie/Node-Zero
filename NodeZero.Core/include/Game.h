#pragma once

#include <memory>
#include <vector>

#include "Events/Subject.h"
#include "IGame.h"
#include "INode.h"
#include "Services/DamageZoneService.h"
#include "Services/HealthService.h"
#include "Services/LevelService.h"
#include "Services/PickupService.h"
#include "Services/SpawnService.h"
#include "Services/UpgradeService.h"
#include "Types/PointPickup.h"

class Game : public IGame {
   private:
    Subject m_Subject;
    std::vector<INode*> m_Nodes;
    float m_ScreenWidth;
    float m_ScreenHeight;
    float m_ElapsedTime;

    int m_NodesDestroyed;
    int m_HighPoints;

    INode* m_Boss;

    float m_MouseX;
    float m_MouseY;
    std::vector<PointPickup> m_CollectedPickupsThisFrame;

    UpgradeService m_UpgradeService;
    PickupService m_PickupService;
    HealthService m_HealthService;
    SpawnService m_SpawnService;
    LevelService m_LevelService;
    DamageZoneService m_DamageZoneService;

   public:
    Game();
    ~Game();

    void Initialize(float screenWidth, float screenHeight) override;
    void Update(float deltaTime) override;
    void Reset() override;
    void StartNextLevel() override;

    void SetMousePosition(float x, float y) override;
    void SpawnNode(float x, float y) override;

    float GetScreenWidth() const override;
    float GetScreenHeight() const override;

    const std::vector<INode*>& GetNodes() const override;
    std::vector<PointPickup> GetCollectedPickupsThisFrame() const override;

    int GetNodesDestroyed() const override;
    void SaveProgress() override;
    int GetHighPoints() const override;

    UpgradeService& GetUpgradeService() override;
    PickupService& GetPickupService() override;
    HealthService& GetHealthService() override;
    LevelService& GetLevelService() override;
    DamageZoneService& GetDamageZoneService() override;
    SpawnService& GetSpawnService() override;

    void Attach(std::shared_ptr<IObserver> observer) override;
    void Detach(std::shared_ptr<IObserver> observer) override;
    void Notify(const std::shared_ptr<IEvent>& event) override;

   private:
    INode* CreateNode(NodeShape shape, float size, float speed);
    void SpawnBoss();
    void OnNodeSpawned(float x, float y, NodeShape shape, float dirX, float dirY);
};
