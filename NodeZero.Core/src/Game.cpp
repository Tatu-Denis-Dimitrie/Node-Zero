#include "Game.h"

#include <algorithm>

#include "../include/Events/GameEvents.h"
#include "Node.h"

Game::Game()
    : m_ScreenWidth(0.0f), m_ScreenHeight(0.0f), m_ElapsedTime(0.0f) {
}

Game::~Game() {
    for (INode* node : m_Nodes) {
        delete node;
    }
    m_Nodes.clear();
}

void Game::Initialize(float screenWidth, float screenHeight) {
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
}

void Game::Update(float deltaTime) {
    m_ElapsedTime += deltaTime;

    for (INode* node : m_Nodes) {
        node->Update(deltaTime);
    }

    m_Nodes.erase(
        std::remove_if(m_Nodes.begin(), m_Nodes.end(),
                       [this](INode* node) {
                           bool shouldRemove = node->GetState() == NodeState::Dead ||
                                               node->GetPosition().GetX() < -100.0f;
                           if (shouldRemove) {
                               if (node->GetState() == NodeState::Dead) {
                                   auto event = std::make_shared<NodeDestroyedEvent>(
                                       m_ElapsedTime,
                                       node->GetShape(),
                                       Vector2{node->GetPosition().GetX(), node->GetPosition().GetY()},
                                       100);
                                   m_EventManager.Publish(event);
                               }

                               delete node;
                           }
                           return shouldRemove;
                       }),
        m_Nodes.end());
}

float Game::GetScreenWidth() const {
    return m_ScreenWidth;
}

float Game::GetScreenHeight() const {
    return m_ScreenHeight;
}

const std::vector<INode*>& Game::GetNodes() const {
    return m_Nodes;
}

void Game::SpawnNode(float x, float y) {
    INode* node = CreateNode(NodeShape::Circle, 30.0f, 100.0f);
    node->Spawn(x, y);
    m_Nodes.push_back(node);

    auto event = std::make_shared<NodeSpawnedEvent>(
        m_ElapsedTime,
        node->GetShape(),
        Vector2{x, y},
        node->GetSize(),
        static_cast<int>(node->GetHP()));
    m_EventManager.Publish(event);
}

EventManager& Game::GetEventManager() {
    return m_EventManager;
}

INode* Game::CreateNode(NodeShape shape, float size, float speed) {
    return new Node(shape, size, speed);
}
