#include "Game.h"
#include "Node.h"
#include <algorithm>

Game::Game()
    : m_ScreenWidth(0.0f)
    , m_ScreenHeight(0.0f)
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
}

INode* Game::CreateNode(NodeShape shape, float size, float speed)
{
    return new Node(shape, size, speed);
}
