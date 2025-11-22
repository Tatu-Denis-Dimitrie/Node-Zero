#include "Game.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "../include/Events/GameEvents.h"
#include "../include/Systems/SaveSystem.h"
#include "Node.h"

Game::Game()
    : m_ScreenWidth(0.0f), m_ScreenHeight(0.0f), m_ElapsedTime(0.0f), m_NextPickupId(0), m_PickupScore(0), m_MaxHealth(15.0f), m_CurrentHealth(15.0f), m_HealthDepletionRate(0.1f), m_HealthDepletionInterval(0.3f), m_HealthTimer(0.0f), m_NodesDestroyed(0), m_HighScore(0), m_SpawnTimer(0.0f), m_SpawnInterval(2.0f), m_DamageTimer(0.0f), m_DamageInterval(1.5f), m_DamageZoneSize(50.0f), m_DamagePerTick(40.0f) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SaveData saveData = SaveSystem::LoadProgress();
    m_HighScore = saveData.highScore;
    m_MaxHealth = saveData.maxHealth;
    m_CurrentHealth = m_MaxHealth;
    m_DamageZoneSize = saveData.damageZoneSize;
    m_DamagePerTick = saveData.damagePerTick;
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
                                   Notify(event);
                                   SpawnPointPickups(position);
                                   m_NodesDestroyed++;
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
    Notify(event);
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

void Game::Reset() {
    // Delete all nodes
    for (INode* node : m_Nodes) {
        delete node;
    }
    m_Nodes.clear();

    m_ElapsedTime = 0.0f;

    m_Pickups.clear();
    m_NextPickupId = 0;
    m_PickupScore = 0;

    m_CurrentHealth = m_MaxHealth;
    m_HealthTimer = 0.0f;

    m_NodesDestroyed = 0;

    m_SpawnTimer = 0.0f;
    m_DamageTimer = 0.0f;
}

INode* Game::CreateNode(NodeShape shape, float size, float speed) {
    return new Node(shape, size, speed);
}

NodeShape Game::GetRandomShape() {
    int chance = std::rand() % 100;

    if (chance < 60) {
        return NodeShape::Square;
    } else if (chance < 90) {
        return NodeShape::Circle;
    } else {
        return NodeShape::Hexagon;
    }
}

void Game::SpawnPointPickups(const Position& origin) {
    int pickupCount = 5 + (std::rand() % 6);

    for (int i = 0; i < pickupCount; ++i) {
        float angle = RandomRange(0.0f, 6.28318530718f);
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
    SaveData saveData = SaveSystem::LoadProgress();

    saveData.gamesPlayed++;

    saveData.totalNodesDestroyed += m_NodesDestroyed;

    saveData.coins += m_PickupScore;

    if (m_PickupScore > saveData.highScore) {
        saveData.highScore = m_PickupScore;
        m_HighScore = m_PickupScore;
    }

    saveData.maxHealth = m_MaxHealth;
    saveData.damageZoneSize = m_DamageZoneSize;
    saveData.damagePerTick = m_DamagePerTick;

    SaveSystem::SaveProgress(saveData);
}

int Game::GetHighScore() const {
    return m_HighScore;
}

bool Game::BuyHealthUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.coins < HEALTH_UPGRADE_COST) {
        return false;
    }

    saveData.coins -= HEALTH_UPGRADE_COST;

    m_MaxHealth += 1.0f;
    saveData.maxHealth = m_MaxHealth;

    if (m_CurrentHealth > 0.0f) {
        m_CurrentHealth = m_MaxHealth;
    }

    SaveSystem::SaveProgress(saveData);

    return true;
}

int Game::GetHealthUpgradeCost() const {
    return HEALTH_UPGRADE_COST;
}

float Game::GetDamageZoneSize() const {
    return m_DamageZoneSize;
}

bool Game::BuyDamageZoneUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.coins < DAMAGE_ZONE_UPGRADE_COST) {
        return false;
    }

    saveData.coins -= DAMAGE_ZONE_UPGRADE_COST;

    m_DamageZoneSize += DAMAGE_ZONE_UPGRADE_AMOUNT;
    saveData.damageZoneSize = m_DamageZoneSize;

    SaveSystem::SaveProgress(saveData);

    return true;
}

int Game::GetDamageZoneUpgradeCost() const {
    return DAMAGE_ZONE_UPGRADE_COST;
}

float Game::GetDamagePerTick() const {
    return m_DamagePerTick;
}

bool Game::BuyDamageUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.coins < DAMAGE_UPGRADE_COST) {
        return false;
    }

    saveData.coins -= DAMAGE_UPGRADE_COST;

    m_DamagePerTick += DAMAGE_UPGRADE_AMOUNT;
    saveData.damagePerTick = m_DamagePerTick;

    SaveSystem::SaveProgress(saveData);

    return true;
}

int Game::GetDamageUpgradeCost() const {
    return DAMAGE_UPGRADE_COST;
}

void Game::UpdateAutoSpawn(float deltaTime) {
    m_SpawnTimer += deltaTime;
    if (m_SpawnTimer >= m_SpawnInterval) {
        m_SpawnTimer = 0.0f;

        float centerX = m_ScreenWidth / 2.0f;
        float centerY = m_ScreenHeight / 2.0f;

        int edge = std::rand() % 4;
        float spawnX, spawnY;

        switch (edge) {
            case 0:
                spawnX = RandomRange(50.0f, m_ScreenWidth - 50.0f);
                spawnY = -50.0f;
                break;
            case 1:
                spawnX = m_ScreenWidth + 50.0f;
                spawnY = RandomRange(50.0f, m_ScreenHeight - 50.0f);
                break;
            case 2:
                spawnX = RandomRange(50.0f, m_ScreenWidth - 50.0f);
                spawnY = m_ScreenHeight + 50.0f;
                break;
            case 3:
                spawnX = -50.0f;
                spawnY = RandomRange(50.0f, m_ScreenHeight - 50.0f);
                break;
            default:
                spawnX = centerX;
                spawnY = -50.0f;
                break;
        }

        SpawnNode(spawnX, spawnY);

        if (!m_Nodes.empty()) {
            INode* lastNode = m_Nodes.back();

            float offsetRange = 150.0f;
            float targetX = centerX + RandomRange(-offsetRange, offsetRange);
            float targetY = centerY + RandomRange(-offsetRange, offsetRange);

            float dirX = targetX - spawnX;
            float dirY = targetY - spawnY;

            float length = std::sqrt(dirX * dirX + dirY * dirY);
            if (length > 0.0f) {
                dirX /= length;
                dirY /= length;
            }

            lastNode->SetDirection(dirX, dirY);
        }
    }
}

void Game::ProcessDamageZone(float centerX, float centerY, float zoneSize, float damage, bool shouldDealDamage) {
    if (!shouldDealDamage) {
        return;
    }

    float damageRectX = centerX - zoneSize / 2.0f;
    float damageRectY = centerY - zoneSize / 2.0f;

    for (INode* node : m_Nodes) {
        if (node->GetState() != NodeState::Active)
            continue;

        float nodeX = node->GetPosition().x;
        float nodeY = node->GetPosition().y;
        float nodeSize = node->GetSize();

        bool inDamageZone = !(nodeX + nodeSize < damageRectX ||
                              nodeX - nodeSize > damageRectX + zoneSize ||
                              nodeY + nodeSize < damageRectY ||
                              nodeY - nodeSize > damageRectY + zoneSize);

        if (inDamageZone) {
            node->TakeDamage(damage);
            ReduceHealth(0.5f);
        }
    }
}

void Game::UpdateDamageTimer(float deltaTime) {
    m_DamageTimer += deltaTime;
}

bool Game::ShouldDealDamage() const {
    return m_DamageTimer >= m_DamageInterval;
}

void Game::ResetDamageTimer() {
    m_DamageTimer = 0.0f;
}

void Game::ProcessPickupCollection(float centerX, float centerY, float zoneSize) {
    float collectRectX = centerX - zoneSize / 2.0f;
    float collectRectY = centerY - zoneSize / 2.0f;

    static constexpr float PICKUP_COLLECT_DELAY = 1.0f;

    for (const PointPickup& pickup : m_Pickups) {
        if (pickup.GetAge() < PICKUP_COLLECT_DELAY) {
            continue;
        }

        bool intersects = !(pickup.position.x + pickup.size < collectRectX ||
                            pickup.position.x - pickup.size > collectRectX + zoneSize ||
                            pickup.position.y + pickup.size < collectRectY ||
                            pickup.position.y - pickup.size > collectRectY + zoneSize);

        if (intersects) {
            CollectPickup(pickup.id);
        }
    }
}

bool Game::ShouldGameOver() const {
    return IsGameOver();
}

void Game::Attach(std::shared_ptr<IObserver> observer) {
    m_observers.push_back(observer);
}

void Game::Detach(std::shared_ptr<IObserver> observer) {
    m_observers.remove(observer);
}

void Game::Notify(const std::shared_ptr<IEvent>& event) {
    for (const auto& observer : m_observers) {
        observer->Update(event);
    }
}
