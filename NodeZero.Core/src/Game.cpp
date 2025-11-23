#include "Game.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "../include/Config/GameConfig.h"
#include "../include/Events/GameEvents.h"
#include "../include/Systems/SaveSystem.h"
#include "Node.h"

Game::Game()
    : m_ScreenWidth(0.0f), m_ScreenHeight(0.0f), m_ElapsedTime(0.0f), m_NextPickupId(0), m_PickupScore(0), m_MaxHealth(15.0f), m_CurrentHealth(15.0f), m_RegenRate(0.0f), m_HealthDepletionRate(0.1f), m_HealthDepletionInterval(0.3f), m_HealthTimer(0.0f), m_NodesDestroyed(0), m_HighScore(0), m_SpawnTimer(0.0f), m_SpawnInterval(2.0f), m_DamageTimer(0.0f), m_DamageInterval(1.5f), m_DamageZoneSize(50.0f), m_DamagePerTick(40.0f), m_ProgressBarPercentage(0.0f), m_CurrentLevel(1), m_NodesDestroyedThisLevel(0), m_BossActive(false), m_Boss(nullptr), m_LevelTimer(0.0f), m_LevelDuration(GameConfig::LEVEL_DURATION) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SaveData saveData = SaveSystem::LoadProgress();
    m_HighScore = saveData.highScore;
    m_MaxHealth = saveData.maxHealth;
    m_RegenRate = saveData.regenRate;
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

    // Update level timer and progress bar based on time
    if (!m_BossActive) {
        m_LevelTimer += deltaTime;
        m_ProgressBarPercentage = (m_LevelTimer / m_LevelDuration) * 100.0f;

        // Clamp to 100%
        if (m_ProgressBarPercentage > 100.0f) {
            m_ProgressBarPercentage = 100.0f;
        }
    }

    for (INode* node : m_Nodes) {
        node->Update(deltaTime);
    }

    // Track if we need to spawn boss after removal
    bool shouldSpawnBoss = false;

    m_Nodes.erase(
        std::remove_if(m_Nodes.begin(), m_Nodes.end(),
                       [this, &shouldSpawnBoss](INode* node) {
                           bool shouldRemove = node->GetState() == NodeState::Dead ||
                                               node->GetPosition().x < -100.0f;
                           if (shouldRemove) {
                               if (node->GetState() == NodeState::Dead) {
                                   Position position{node->GetPosition().x, node->GetPosition().y};
                                   bool isBoss = node->GetShape() == NodeShape::Boss;

                                   if (isBoss) {
                                       int scoreGained = 500 * m_CurrentLevel;
                                       auto event = std::make_shared<BossDefeatedEvent>(
                                           m_ElapsedTime,
                                           m_CurrentLevel,
                                           scoreGained);
                                       Notify(event);

                                       m_BossActive = false;
                                       m_Boss = nullptr;
                                       AdvanceToNextLevel();
                                   } else {
                                       auto event = std::make_shared<NodeDestroyedEvent>(
                                           m_ElapsedTime,
                                           node->GetShape(),
                                           position,
                                           100);
                                       Notify(event);
                                       SpawnPointPickups(position);
                                       m_NodesDestroyed++;
                                       m_NodesDestroyedThisLevel++;
                                   }
                               }

                               delete node;
                           }
                           return shouldRemove;
                       }),
        m_Nodes.end());

    // Spawn boss AFTER removal is complete
    if (shouldSpawnBoss) {
        SpawnBoss();
    }

    // Check if level timer reached 100% and spawn boss
    if (m_LevelTimer >= m_LevelDuration && !m_BossActive) {
        SpawnBoss();
    }

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
    INode* node = CreateNode(shape, GameConfig::NODE_DEFAULT_SIZE, GameConfig::NODE_DEFAULT_SPEED);
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
    if (age < GameConfig::PICKUP_COLLECT_DELAY) {
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

    m_ProgressBarPercentage = 0.0f;

    // Reset level and boss state
    m_CurrentLevel = 1;
    m_NodesDestroyedThisLevel = 0;
    m_BossActive = false;
    m_Boss = nullptr;
    m_LevelTimer = 0.0f;
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
        pickup.size = GameConfig::PICKUP_SIZE;
        pickup.lifetime = GameConfig::PICKUP_LIFETIME;
        pickup.remainingTime = GameConfig::PICKUP_LIFETIME;
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
    // Apply regeneration
    if (m_RegenRate > 0.0f && m_CurrentHealth < m_MaxHealth && m_CurrentHealth > 0.0f) {
        m_CurrentHealth += m_RegenRate * deltaTime;
        if (m_CurrentHealth > m_MaxHealth) {
            m_CurrentHealth = m_MaxHealth;
        }
    }

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
    saveData.regenRate = m_RegenRate;
    saveData.damageZoneSize = m_DamageZoneSize;
    saveData.damagePerTick = m_DamagePerTick;

    SaveSystem::SaveProgress(saveData);
}

int Game::GetHighScore() const {
    return m_HighScore;
}

bool Game::BuyHealthUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.coins < GameConfig::HEALTH_UPGRADE_COST) {
        return false;
    }

    saveData.coins -= GameConfig::HEALTH_UPGRADE_COST;

    m_MaxHealth += 1.0f;
    saveData.maxHealth = m_MaxHealth;

    if (m_CurrentHealth > 0.0f) {
        m_CurrentHealth = m_MaxHealth;
    }

    SaveSystem::SaveProgress(saveData);

    return true;
}

int Game::GetHealthUpgradeCost() const {
    return GameConfig::HEALTH_UPGRADE_COST;
}

bool Game::BuyRegenUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.coins < GameConfig::REGEN_UPGRADE_COST) {
        return false;
    }

    saveData.coins -= GameConfig::REGEN_UPGRADE_COST;

    m_RegenRate += GameConfig::REGEN_UPGRADE_AMOUNT;
    saveData.regenRate = m_RegenRate;

    SaveSystem::SaveProgress(saveData);

    return true;
}

int Game::GetRegenUpgradeCost() const {
    return GameConfig::REGEN_UPGRADE_COST;
}

float Game::GetRegenRate() const {
    return m_RegenRate;
}

float Game::GetDamageZoneSize() const {
    return m_DamageZoneSize;
}

bool Game::BuyDamageZoneUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.coins < GameConfig::DAMAGE_ZONE_UPGRADE_COST) {
        return false;
    }

    saveData.coins -= GameConfig::DAMAGE_ZONE_UPGRADE_COST;

    m_DamageZoneSize += GameConfig::DAMAGE_ZONE_UPGRADE_AMOUNT;
    saveData.damageZoneSize = m_DamageZoneSize;

    SaveSystem::SaveProgress(saveData);

    return true;
}

int Game::GetDamageZoneUpgradeCost() const {
    return GameConfig::DAMAGE_ZONE_UPGRADE_COST;
}

float Game::GetDamagePerTick() const {
    return m_DamagePerTick;
}

bool Game::BuyDamageUpgrade() {
    SaveData saveData = SaveSystem::LoadProgress();

    if (saveData.coins < GameConfig::DAMAGE_UPGRADE_COST) {
        return false;
    }

    saveData.coins -= GameConfig::DAMAGE_UPGRADE_COST;

    m_DamagePerTick += GameConfig::DAMAGE_UPGRADE_AMOUNT;
    saveData.damagePerTick = m_DamagePerTick;

    SaveSystem::SaveProgress(saveData);

    return true;
}

int Game::GetDamageUpgradeCost() const {
    return GameConfig::DAMAGE_UPGRADE_COST;
}

float Game::GetProgressBarPercentage() const {
    return m_ProgressBarPercentage;
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

std::vector<PointPickup> Game::ProcessPickupCollection(float centerX, float centerY, float zoneSize) {
    float collectRectX = centerX - zoneSize / 2.0f;
    float collectRectY = centerY - zoneSize / 2.0f;

    std::vector<PointPickup> collectedPickups;

    // Folosim constanta din Game.h în loc de hardcoded (evită duplicare)
    for (const PointPickup& pickup : m_Pickups) {
        if (pickup.GetAge() < GameConfig::PICKUP_COLLECT_DELAY) {
            continue;
        }

        bool intersects = !(pickup.position.x + pickup.size < collectRectX ||
                            pickup.position.x - pickup.size > collectRectX + zoneSize ||
                            pickup.position.y + pickup.size < collectRectY ||
                            pickup.position.y - pickup.size > collectRectY + zoneSize);

        if (intersects) {
            collectedPickups.push_back(pickup);  // Salvăm pickup-ul înainte de colectare
            CollectPickup(pickup.id);
        }
    }

    return collectedPickups;
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

void Game::SpawnBoss() {
    if (m_BossActive) return;

    // Boss HP scales aggressively: 200 + (level-1) * 100
    // Level 1: 200, Level 2: 300, Level 3: 400, etc.
    float bossHP = GameConfig::BOSS_HP_BASE + (m_CurrentLevel - 1) * 100.0f;
    m_Boss = CreateNode(NodeShape::Boss, GameConfig::BOSS_SIZE, GameConfig::BOSS_SPEED);

    // Cast to Node* to call SetHP
    Node* bossNode = static_cast<Node*>(m_Boss);
    bossNode->SetHP(bossHP);

    m_Boss->Spawn(m_ScreenWidth, m_ScreenHeight / 2.0f);
    m_Boss->SetDirection(-1.0f, 0.0f);
    m_Nodes.push_back(m_Boss);
    m_BossActive = true;

    auto event = std::make_shared<BossSpawnedEvent>(m_ElapsedTime, m_CurrentLevel, bossHP);
    Notify(event);
}

void Game::AdvanceToNextLevel() {
    int oldLevel = m_CurrentLevel;
    m_CurrentLevel++;
    m_NodesDestroyedThisLevel = 0;

    // Reset level timer and progress bar for next level
    m_LevelTimer = 0.0f;
    m_ProgressBarPercentage = 0.0f;

    auto event = std::make_shared<LevelCompletedEvent>(m_ElapsedTime, oldLevel, m_CurrentLevel);
    Notify(event);
}

int Game::GetCurrentLevel() const {
    return m_CurrentLevel;
}

int Game::GetNodesDestroyedThisLevel() const {
    return m_NodesDestroyedThisLevel;
}
