#include "Game.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Config/GameConfig.h"
#include "Events/GameEvents.h"

Game::Game()
    : m_ScreenWidth(0.0f),
      m_ScreenHeight(0.0f),
      m_ElapsedTime(0.0f),
      m_NodesDestroyed(0),
      m_HighPoints(0),
      m_Boss(nullptr),
      m_MouseX(0.0f),
      m_MouseY(0.0f) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SaveData saveData = m_SaveService.LoadProgress();
    m_HighPoints = saveData.highPoints;

    m_UpgradeService.SetSaveService(&m_SaveService);
    m_UpgradeService.Initialize(saveData.maxHealth, saveData.regenRate, saveData.damageZoneSize, saveData.damagePerTick);
    m_HealthService.Initialize(saveData.maxHealth, saveData.regenRate);
    m_LevelService.Initialize(saveData.currentLevel);
}

Game::~Game() {
    for (Node* node : m_Nodes) {
        delete node;
    }
    m_Nodes.clear();
}

void Game::Initialize(float screenWidth, float screenHeight) {
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;

    m_PickupService.Initialize(screenHeight);
    m_SpawnService.Initialize(screenWidth, screenHeight);
    m_SpawnService.SetCurrentLevel(m_LevelService.GetCurrentLevel());

    auto spawnCallback = [this](float x, float y, NodeShape shape, float dirX, float dirY) {
        OnNodeSpawned(x, y, shape, dirX, dirY);
    };
    m_SpawnService.SetOnNodeSpawnedCallback(spawnCallback);
}

void Game::Update(float deltaTime) {
    m_CollectedPickupsThisFrame.clear();

    m_HealthService.Update(deltaTime);
    m_HealthService.SetCurrentLevel(m_LevelService.GetCurrentLevel());

    m_SpawnService.UpdateAutoSpawn(deltaTime);
    m_SpawnService.SetCurrentLevel(m_LevelService.GetCurrentLevel());

    m_DamageZoneService.UpdateTimer(deltaTime);

    bool shouldDealDamage = m_DamageZoneService.ShouldDealDamage();
    if (shouldDealDamage) {
        m_DamageZoneService.ResetTimer();

        auto onNodeDamaged = [this](Node* node, float healthCost) {
            Position nodePos = node->GetPosition();
            auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::NodeDamaged);
            event->position = nodePos;
            event->damage = static_cast<int>(m_UpgradeService.GetDamagePerTick());
            event->hp = static_cast<int>(node->GetHP());
            Notify(event);

            m_HealthService.Reduce(healthCost);
        };

        m_DamageZoneService.ProcessDamageZone(
            m_MouseX,
            m_MouseY,
            m_UpgradeService.GetDamageZoneSize(),
            m_UpgradeService.GetDamagePerTick(),
            m_LevelService.GetCurrentLevel(),
            m_Nodes,
            onNodeDamaged);
    }

    m_CollectedPickupsThisFrame = m_PickupService.ProcessPickupCollection(m_MouseX, m_MouseY, m_UpgradeService.GetDamageZoneSize());

    m_ElapsedTime += deltaTime;

    m_LevelService.Update(deltaTime, m_LevelService.IsBossActive());

    for (Node* node : m_Nodes) {
        node->Update(deltaTime);
    }

    m_Nodes.erase(
        std::remove_if(m_Nodes.begin(), m_Nodes.end(),
                       [this](Node* node) {
                           bool isBoss = node->GetShape() == NodeShape::Boss;
                           bool isOffScreen = node->GetPosition().x < -200.0f;

                           if (isBoss) {
                               isOffScreen = false;
                           }

                           bool shouldRemove = node->GetState() == NodeState::Dead || isOffScreen;

                           if (shouldRemove) {
                               if (node->GetState() == NodeState::Dead) {
                                   Position position{node->GetPosition().x, node->GetPosition().y};

                                   if (isBoss) {
                                       int pointsGained = 500 * m_LevelService.GetCurrentLevel();
                                       auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::BossDefeated);
                                       event->level = m_LevelService.GetCurrentLevel();
                                       event->points = pointsGained;
                                       Notify(event);

                                       m_LevelService.SetBossActive(false);
                                       m_Boss = nullptr;
                                       m_LevelService.SetLevelCompleted(true);
                                   } else {
                                       auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::NodeDestroyed);
                                       event->shape = node->GetShape();
                                       event->position = position;
                                       event->points = 100;
                                       Notify(event);
                                       m_PickupService.SpawnPointPickups(position);
                                       m_NodesDestroyed++;
                                       m_LevelService.IncrementNodesDestroyed();
                                   }
                               }

                               delete node;
                           }
                           return shouldRemove;
                       }),
        m_Nodes.end());

    if (m_LevelService.ShouldSpawnBoss()) {
        SpawnBoss();
    }

    m_PickupService.Update(deltaTime);
}

float Game::GetScreenWidth() const {
    return m_ScreenWidth;
}

float Game::GetScreenHeight() const {
    return m_ScreenHeight;
}

const std::vector<INode*>& Game::GetNodes() const {
    return reinterpret_cast<const std::vector<INode*>&>(m_Nodes);
}

void Game::SpawnNode(float x, float y) {
    NodeShape shape = m_SpawnService.GetRandomShape();
    float nodeSize = m_ScreenHeight * 0.0375f;
    Node* node = CreateNode(shape, nodeSize, GameConfig::NODE_DEFAULT_SPEED);

    float baseHP = node->GetHP();
    float scaledHP = m_SpawnService.CalculateNodeHP(baseHP);
    node->SetHP(scaledHP);

    node->Spawn(x, y);
    m_Nodes.push_back(node);

    auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::NodeSpawned);
    event->shape = node->GetShape();
    event->position = Position{x, y};
    event->size = node->GetSize();
    event->hp = static_cast<int>(node->GetHP());
    Notify(event);
}

void Game::Reset() {
    for (Node* node : m_Nodes) {
        delete node;
    }
    m_Nodes.clear();

    m_ElapsedTime = 0.0f;

    m_PickupService.Reset();
    m_HealthService.Reset(m_UpgradeService.GetMaxHealth());

    m_NodesDestroyed = 0;

    m_SpawnService.Reset();
    m_DamageZoneService.ResetTimer();

    SaveData saveData = m_SaveService.LoadProgress();
    m_LevelService.Reset(saveData.currentLevel);

    m_Boss = nullptr;
}

Node* Game::CreateNode(NodeShape shape, float size, float speed) {
    return new Node(shape, size, speed);
}

int Game::GetNodesDestroyed() const {
    return m_NodesDestroyed;
}

void Game::SaveProgress() {
    SaveData saveData = m_SaveService.LoadProgress();

    saveData.totalNodesDestroyed += m_NodesDestroyed;
    saveData.points += m_PickupService.GetPickupPoints();

    if (m_PickupService.GetPickupPoints() > saveData.highPoints) {
        saveData.highPoints = m_PickupService.GetPickupPoints();
        m_HighPoints = m_PickupService.GetPickupPoints();
    }

    saveData.currentLevel = m_LevelService.GetCurrentLevel();
    saveData.maxHealth = m_UpgradeService.GetMaxHealth();
    saveData.regenRate = m_UpgradeService.GetRegenRate();
    saveData.damageZoneSize = m_UpgradeService.GetDamageZoneSize();
    saveData.damagePerTick = m_UpgradeService.GetDamagePerTick();

    m_SaveService.SaveProgress(saveData);
}

int Game::GetHighPoints() const {
    return m_HighPoints;
}

IUpgradeService& Game::GetUpgradeService() {
    return m_UpgradeService;
}

IPickupService& Game::GetPickupService() {
    return m_PickupService;
}

IHealthService& Game::GetHealthService() {
    return m_HealthService;
}

ILevelService& Game::GetLevelService() {
    return m_LevelService;
}

IDamageZoneService& Game::GetDamageZoneService() {
    return m_DamageZoneService;
}

ISpawnService& Game::GetSpawnService() {
    return m_SpawnService;
}

ISaveService& Game::GetSaveService() {
    return m_SaveService;
}

void Game::Attach(std::shared_ptr<IObserver> observer) {
    m_Subject.Attach(observer);
}

void Game::Detach(std::shared_ptr<IObserver> observer) {
    m_Subject.Detach(observer);
}

void Game::Notify(const std::shared_ptr<IEvent>& event) {
    m_Subject.Notify(event);
}

void Game::SpawnBoss() {
    if (m_LevelService.IsBossActive()) return;

    float bossSize = m_ScreenHeight * 0.15f;
    float bossHP = GameConfig::BOSS_HP_BASE + (m_LevelService.GetCurrentLevel() - 1) * 100.0f;
    m_Boss = CreateNode(NodeShape::Boss, bossSize, GameConfig::BOSS_SPEED);

    m_Boss->SetHP(bossHP);

    float spawnX, spawnY;
    int edge = std::rand() % 4;
    float offset = bossSize * 1.5f;

    switch (edge) {
        case 0:
            spawnX = (static_cast<float>(std::rand()) / RAND_MAX) * m_ScreenWidth;
            spawnY = -offset;
            break;
        case 1:
            spawnX = m_ScreenWidth + offset;
            spawnY = (static_cast<float>(std::rand()) / RAND_MAX) * m_ScreenHeight;
            break;
        case 2:
            spawnX = (static_cast<float>(std::rand()) / RAND_MAX) * m_ScreenWidth;
            spawnY = m_ScreenHeight + offset;
            break;
        case 3:
            spawnX = -offset;
            spawnY = (static_cast<float>(std::rand()) / RAND_MAX) * m_ScreenHeight;
            break;
    }

    m_Boss->Spawn(spawnX, spawnY);

    float centerX = m_ScreenWidth / 2.0f;
    float centerY = m_ScreenHeight / 2.0f;
    float dirX = centerX - spawnX;
    float dirY = centerY - spawnY;

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0) {
        dirX /= length;
        dirY /= length;
    }

    m_Boss->SetDirection(dirX, dirY);
    m_Nodes.push_back(m_Boss);
    m_LevelService.SetBossActive(true);

    auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::BossSpawned);
    event->level = m_LevelService.GetCurrentLevel();
    event->bossHP = bossHP;
    Notify(event);
}

void Game::StartNextLevel() {
    int oldLevel = m_LevelService.GetCurrentLevel();
    m_LevelService.StartNextLevel();

    SaveProgress();

    for (Node* node : m_Nodes) {
        delete node;
    }
    m_Nodes.clear();

    m_PickupService.Reset();

    m_SpawnService.Reset();
    m_SpawnService.SetCurrentLevel(m_LevelService.GetCurrentLevel());

    m_HealthService.RestoreToMax();

    m_Boss = nullptr;

    auto event = std::make_shared<GameEvent>(m_ElapsedTime, EventType::LevelCompleted);
    event->level = oldLevel;
    event->nextLevel = m_LevelService.GetCurrentLevel();
    Notify(event);
}

void Game::SetMousePosition(float x, float y) {
    m_MouseX = x;
    m_MouseY = y;
}

std::vector<PointPickup> Game::GetCollectedPickupsThisFrame() const {
    return m_CollectedPickupsThisFrame;
}

void Game::OnNodeSpawned(float x, float y, NodeShape shape, float dirX, float dirY) {
    SpawnNode(x, y);

    if (!m_Nodes.empty()) {
        Node* lastNode = m_Nodes.back();
        lastNode->SetDirection(dirX, dirY);
    }
}
