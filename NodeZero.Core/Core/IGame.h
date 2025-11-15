#pragma once

#include "IPlayer.h"
#include "IEnemy.h"
#include "../Enums/EnemyTypes.h"
#include <vector>

class IGame
{
public:
    virtual ~IGame() = default;

    virtual void Initialize(float screenWidth, float screenHeight) = 0;
    virtual void Update(float deltaTime) = 0;
    
    virtual IPlayer& GetPlayer() = 0;
    virtual const IPlayer& GetPlayer() const = 0;
    
    virtual const std::vector<IEnemy*>& GetEnemies() const = 0;
    virtual void SpawnEnemy(EnemyShape shape, float x, float y, float size, float speed) = 0;
};
