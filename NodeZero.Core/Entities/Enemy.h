#pragma once

#include "../Core/IEnemy.h"
#include "Position.h"

class Enemy : public IEnemy
{
private:
    Position m_Position;
    EnemyShape m_Shape;
    EnemyState m_State;
    float m_Size;
    float m_Speed;

public:
    Enemy(EnemyShape shape, float size, float speed);

    IPosition& GetPosition() override;
    const IPosition& GetPosition() const override;
    
    EnemyShape GetShape() const override;
    EnemyState GetState() const override;
    
    float GetSize() const override;
    float GetSpeed() const override;
    
    void Spawn(float x, float y) override;
    void Kill() override;
    void Update(float deltaTime) override;
};
