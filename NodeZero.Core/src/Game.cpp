#include "../include/IGame.h"
#include "../include/INode.h"
#include <algorithm>
#include <vector>

#include "Node.cpp"

class Game : public IGame
{
private:
    std::vector<INode*> m_Nodes;
    float m_ScreenWidth;
    float m_ScreenHeight;

public:
    Game()
        : m_ScreenWidth(0.0f)
        , m_ScreenHeight(0.0f)
    {
    }

    ~Game()
    {
        for (INode* node : m_Nodes)
        {
            delete node;
        }
        m_Nodes.clear();
    }

    void Initialize(float screenWidth, float screenHeight) override
    {
        m_ScreenWidth = screenWidth;
        m_ScreenHeight = screenHeight;
    }

    void Update(float deltaTime) override
    {
        for (INode* node : m_Nodes)
        {
            node->Update(deltaTime);
        }

        m_Nodes.erase(
            std::remove_if(m_Nodes.begin(), m_Nodes.end(),
                [this](INode* node) {
                    bool shouldRemove = node->GetState() == NodeState::Dead ||
                                      node->GetPosition().GetX() < -100.0f;
                    if (shouldRemove)
                    {
                        delete node;
                    }
                    return shouldRemove;
                }),
            m_Nodes.end()
        );
    }

    float GetScreenWidth() const override
    {
        return m_ScreenWidth;
    }

    float GetScreenHeight() const override
    {
        return m_ScreenHeight;
    }

    const std::vector<INode*>& GetNodes() const override
    {
        return m_Nodes;
    }

    void SpawnNode(float x, float y) override
    {
        INode* node = CreateNode(NodeShape::Circle, 30.0f, 100.0f);
        node->Spawn(x, y);
        m_Nodes.push_back(node);
    }

private:
    INode* CreateNode(NodeShape shape, float size, float speed)
    {
        return new Node(shape, size, speed);
    }
};
