#include "../include/IGame.h"
#include "../include/INode.h"
#include <algorithm>
#include <vector>

// Include Node implementation (care include și Position)
// În viitor, acest cod ar trebui să fie într-un factory separat
#include "Node.cpp"

/// <summary>
/// Implementare concretă a jocului
/// </summary>
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
        // Cleanup
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
        // Actualizează toate nodurile
        for (INode* node : m_Nodes)
        {
            node->Update(deltaTime);
        }

        // Șterge nodurile moarte sau care au ieșit din ecran
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
        // Creează un nod nou (pentru moment, folosim valori default)
        // Într-o implementare completă, ar fi configurat din parametri
        INode* node = CreateNode(NodeShape::Circle, 30.0f, 100.0f);
        node->Spawn(x, y);
        m_Nodes.push_back(node);
    }

private:
    // Helper pentru a crea noduri - va fi mutat în factory pattern
    INode* CreateNode(NodeShape shape, float size, float speed)
    {
        return new Node(shape, size, speed);
    }
};
