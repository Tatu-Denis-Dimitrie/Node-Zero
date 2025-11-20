#include "Game.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "../include/Events/GameEvents.h"
#include "../include/Systems/SaveSystem.h"
#include "Node.h"

Game::Game()
    : m_ScreenWidth(0.0f), m_ScreenHeight(0.0f), m_ElapsedTime(0.0f), m_NextPickupId(0), m_PickupScore(0),
      m_MaxHealth(15.0f), m_CurrentHealth(15.0f), m_HealthDepletionRate(0.1f),
      m_HealthDepletionInterval(0.3f), m_HealthTimer(0.0f), m_NodesDestroyed(0), m_HighScore(0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Load save data
    SaveData saveData = SaveSystem::LoadProgress();
    m_HighScore = saveData.highScore;
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
                                               node->GetPosition().x < -100.0f;
                           if (shouldRemove) {
                               if (node->GetState() == NodeState::Dead) {
                                   Position position{node->GetPosition().x, node->GetPosition().y};
                                   auto event = std::make_shared<NodeDestroyedEvent>(
                                       m_ElapsedTime,
                                       node->GetShape(),
                                       position,
                                       100);
                                   m_EventManager.Publish(event);
                                   SpawnPointPickups(position);
                                   m_NodesDestroyed++;  // Increment nodes destroyed counter
                               }

                               delete node;
                           }
                           return shouldRemove;
                       }),
        m_Nodes.end());

    UpdatePickups(deltaTime);
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

const std::vector<PointPickup>& Game::GetPickups() const {
    return m_Pickups;
}

void Game::SpawnNode(float x, float y) {
    NodeShape shape = GetRandomShape();
    INode* node = CreateNode(shape, 30.0f, 100.0f);
    node->Spawn(x, y);
    m_Nodes.push_back(node);

    auto event = std::make_shared<NodeSpawnedEvent>(
        m_ElapsedTime,
        node->GetShape(),
        Position{x, y},
        node->GetSize(),
        static_cast<int>(node->GetHP()));
    m_EventManager.Publish(event);
}

bool Game::CollectPickup(int pickupId) {
    auto it = std::find_if(m_Pickups.begin(), m_Pickups.end(),
                           [pickupId](const PointPickup& pickup) {
                               return pickup.id == pickupId;
                           });

    if (it == m_Pickups.end()) {
        return false;
    }

    float age = it->GetAge();
    if (age < PICKUP_COLLECT_DELAY) {
        return false;
    }

    m_PickupScore += it->points;
    m_Pickups.erase(it);
    return true;
}

EventManager& Game::GetEventManager() {
    return m_EventManager;
}

void Game::Reset() {
    // Delete all nodes
    for (INode* node : m_Nodes) {
        delete node;
    }
    m_Nodes.clear();

    // Reset elapsed time
    m_ElapsedTime = 0.0f;

    // Reset pickups and score
    m_Pickups.clear();
    m_NextPickupId = 0;
    m_PickupScore = 0;

    // Reset health
    m_CurrentHealth = m_MaxHealth;
    m_HealthTimer = 0.0f;

    // Reset nodes destroyed counter
    m_NodesDestroyed = 0;
}

INode* Game::CreateNode(NodeShape shape, float size, float speed) {
    return new Node(shape, size, speed);
}

NodeShape Game::GetRandomShape() {
    int chance = std::rand() % 100;

    if (chance < 60)  // 60% chance for Square
    {
        return NodeShape::Square;
    } else if (chance < 90)  // 30% chance for Circle
    {
        return NodeShape::Circle;
    } else  // 10% chance for Hexagon
    {
        return NodeShape::Hexagon;
    }
}

void Game::SpawnPointPickups(const Position& origin) {
    int pickupCount = 5 + (std::rand() % 6);  // 5 to 10

    for (int i = 0; i < pickupCount; ++i) {
        float angle = RandomRange(0.0f, 6.28318530718f);  // 2 * PI
        float radius = RandomRange(10.0f, 40.0f);

        PointPickup pickup{};
        pickup.id = m_NextPickupId++;
        pickup.position.x = origin.x + std::cos(angle) * radius;
        pickup.position.y = origin.y + std::sin(angle) * radius;
        pickup.spawnOrigin = origin;
        pickup.size = PICKUP_SIZE;
        pickup.lifetime = PICKUP_LIFETIME;
        pickup.remainingTime = PICKUP_LIFETIME;
        pickup.points = 1;

        m_Pickups.push_back(pickup);
    }
}

void Game::UpdatePickups(float deltaTime) {
    for (auto& pickup : m_Pickups) {
        pickup.remainingTime -= deltaTime;
    }

    m_Pickups.erase(std::remove_if(m_Pickups.begin(), m_Pickups.end(),
                                   [](const PointPickup& pickup) {
                                       return pickup.remainingTime <= 0.0f;
                                   }),
                    m_Pickups.end());
}

float Game::RandomRange(float minValue, float maxValue) const {
    float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    return minValue + (maxValue - minValue) * t;
}

int Game::GetPickupScore() const {
    return m_PickupScore;
}

float Game::GetCurrentHealth() const {
    return m_CurrentHealth;
}

float Game::GetMaxHealth() const {
    return m_MaxHealth;
}

void Game::ReduceHealth(float amount) {
    m_CurrentHealth -= amount;
    if (m_CurrentHealth < 0.0f) {
        m_CurrentHealth = 0.0f;
    }
}

void Game::UpdateHealth(float deltaTime) {
    m_HealthTimer += deltaTime;
    if (m_HealthTimer >= m_HealthDepletionInterval) {
        m_CurrentHealth -= m_HealthDepletionRate;
        m_HealthTimer = 0.0f;

        if (m_CurrentHealth < 0.0f) {
            m_CurrentHealth = 0.0f;
        }
    }
}

bool Game::IsGameOver() const {
    return m_CurrentHealth <= 0.0f;
}

int Game::GetNodesDestroyed() const {
    return m_NodesDestroyed;
}

void Game::SaveProgress() {
    // Load existing data
    SaveData saveData = SaveSystem::LoadProgress();

    // Update games played
    saveData.gamesPlayed++;

    // Update total nodes destroyed
    saveData.totalNodesDestroyed += m_NodesDestroyed;

    // Update high score if current score is higher
    if (m_PickupScore > saveData.highScore) {
        saveData.highScore = m_PickupScore;
        m_HighScore = m_PickupScore;
    }

    // Save to file
    SaveSystem::SaveProgress(saveData);
}

int Game::GetHighScore() const {
    return m_HighScore;
}
