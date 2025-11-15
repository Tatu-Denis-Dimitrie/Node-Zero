#pragma once

#include "../include/IGame.h"
#include "../include/INode.h"
#include <vector>

class Game : public IGame
{
private:
    std::vector<INode*> m_Nodes;
    float m_ScreenWidth;
    float m_ScreenHeight;

public:
    Game();
    ~Game();

    void Initialize(float screenWidth, float screenHeight) override;
    void Update(float deltaTime) override;

    float GetScreenWidth() const override;
    float GetScreenHeight() const override;

    const std::vector<INode*>& GetNodes() const override;

    void SpawnNode(float x, float y) override;

private:
    INode* CreateNode(NodeShape shape, float size, float speed);
};
