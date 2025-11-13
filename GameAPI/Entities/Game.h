#pragma once

#include "../Core/IGame.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>

class Game : public IGame
{
private:
    Player m_Player;
    std::vector<IEnemy*> m_Enemies;
    float m_ScreenWidth;
    float m_ScreenHeight;

public:
    Game();
    ~Game();

    void Initialize(float screenWidth, float screenHeight) override;
    void Update(float deltaTime) override;
    
    IPlayer& GetPlayer() override;
    const IPlayer& GetPlayer() const override;
    
    const std::vector<IEnemy*>& GetEnemies() const override;
    void SpawnEnemy(EnemyShape shape, float x, float y, float size, float speed) override;
    
    float GetScreenWidth() const { return m_ScreenWidth; }
    float GetScreenHeight() const { return m_ScreenHeight; }
};
