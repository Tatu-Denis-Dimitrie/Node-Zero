#include "Game.h"
#include <algorithm>

Game::Game()
    : m_Player(), m_ScreenWidth(0.0f), m_ScreenHeight(0.0f)
{
}

Game::~Game()
{
    for (IEnemy* enemy : m_Enemies)
    {
        delete enemy;
    }
    m_Enemies.clear();
}

void Game::Initialize(float screenWidth, float screenHeight)
{
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;
    
    m_Player.SetRotationDegrees(0.0f);
}

void Game::Update(float deltaTime)
{
    m_Player.Update(deltaTime);
    
    for (IEnemy* enemy : m_Enemies)
    {
        enemy->Update(deltaTime);
    }
    
    m_Enemies.erase(
        std::remove_if(m_Enemies.begin(), m_Enemies.end(),
            [this](IEnemy* enemy) {
                bool shouldRemove = enemy->GetState() == EnemyState::Dead ||
                                  enemy->GetPosition().GetX() < -100.0f;
                if (shouldRemove)
                {
                    delete enemy;
                }
                return shouldRemove;
            }),
        m_Enemies.end()
    );
}

IPlayer& Game::GetPlayer()
{
    return m_Player;
}

const IPlayer& Game::GetPlayer() const
{
    return m_Player;
}

const std::vector<IEnemy*>& Game::GetEnemies() const
{
    return m_Enemies;
}

void Game::SpawnEnemy(EnemyShape shape, float x, float y, float size, float speed)
{
    Enemy* enemy = new Enemy(shape, size, speed);
    enemy->Spawn(x, y);
    m_Enemies.push_back(enemy);
}
