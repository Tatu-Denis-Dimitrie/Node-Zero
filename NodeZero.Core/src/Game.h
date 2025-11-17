#pragma once

#include <vector>

#include "../include/Events/EventManager.h"
#include "../include/IGame.h"
#include "../include/INode.h"
#include "../include/Types/PointPickup.h"

class Game : public IGame
{
private:
    std::vector<INode *> m_Nodes;
    std::vector<PointPickup> m_Pickups;
    float m_ScreenWidth;
    float m_ScreenHeight;
    EventManager m_EventManager;
    float m_ElapsedTime;
    int m_NextPickupId;
    int m_PickupScore;

public:
    Game();
    ~Game();

    void Initialize(float screenWidth, float screenHeight) override;
    void Update(float deltaTime) override;

    float GetScreenWidth() const override;
    float GetScreenHeight() const override;

    const std::vector<INode *> &GetNodes() const override;
    const std::vector<PointPickup> &GetPickups() const override;

    void SpawnNode(float x, float y) override;
    bool CollectPickup(int pickupId) override;

    EventManager &GetEventManager() override;

    void Reset() override;

    int GetPickupScore() const override;

private:
    INode *CreateNode(NodeShape shape, float size, float speed);
    NodeShape GetRandomShape();
    void SpawnPointPickups(const Position &origin);
    void UpdatePickups(float deltaTime);
    float RandomRange(float minValue, float maxValue) const;

private:
    static constexpr float PICKUP_LIFETIME = 10.0f;
    static constexpr float PICKUP_SIZE = 6.0f;
    static constexpr float PICKUP_COLLECT_DELAY = 1.0f;
};
