#include "Game.h"
#include "Node.h"
#include "../include/Events/GameEvents.h"
#include <algorithm>

Game::Game()
    : m_ScreenWidth(0.0f)
    , m_ScreenHeight(0.0f)
    , m_ElapsedTime(0.0f)
{
}

Game::~Game()
{
    for (INode* node : m_Nodes)
    {
        delete node;
    }
    m_Nodes.clear();
}

void Game::Initialize(float screenWidth, float screenHeight)
{
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
}

void Game::Update(float deltaTime)
{
    m_ElapsedTime += deltaTime;

    for (INode* node : m_Nodes)
    {
        node->Update(deltaTime);
    }

    // Remove dead or off-screen nodes and publish destroy events
    m_Nodes.erase(
        std::remove_if(m_Nodes.begin(), m_Nodes.end(),
            [this](INode* node) {
                bool shouldRemove = node->GetState() == NodeState::Dead ||
                                  node->GetPosition().GetX() < -100.0f;
                if (shouldRemove)
                {
                    // Publish NodeDestroyedEvent
                    if (node->GetState() == NodeState::Dead) {
                        auto event = std::make_shared<NodeZero::NodeDestroyedEvent>(
                            m_ElapsedTime,
                            node->GetShape(),
                            NodeZero::Vector2{node->GetPosition().GetX(), node->GetPosition().GetY()},
                            100 // Score gained (could be calculated based on node type)
                        );
                        m_EventManager.Publish(event);
                    }

                    delete node;
                }
                return shouldRemove;
            }),
        m_Nodes.end()
    );
}

float Game::GetScreenWidth() const
{
    return m_ScreenWidth;
}

float Game::GetScreenHeight() const
{
    return m_ScreenHeight;
}

const std::vector<INode*>& Game::GetNodes() const
{
    return m_Nodes;
}

void Game::SpawnNode(float x, float y)
{
    INode* node = CreateNode(NodeShape::Circle, 30.0f, 100.0f);
    node->Spawn(x, y);
    m_Nodes.push_back(node);

    // Publish NodeSpawnedEvent
    auto event = std::make_shared<NodeZero::NodeSpawnedEvent>(
        m_ElapsedTime,
        node->GetShape(),
        NodeZero::Vector2{x, y},
        node->GetSize(),
        static_cast<int>(node->GetHP())
    );
    m_EventManager.Publish(event);
}

NodeZero::EventManager& Game::GetEventManager()
{
    return m_EventManager;
}

INode* Game::CreateNode(NodeShape shape, float size, float speed)
{
    return new Node(shape, size, speed);
}
