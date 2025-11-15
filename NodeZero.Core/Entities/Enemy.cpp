#include "Enemy.h"

Enemy::Enemy(EnemyShape shape, float size, float speed)
    : m_Position(0.0f, 0.0f), m_Shape(shape), m_State(EnemyState::Inactive), m_Size(size), m_Speed(speed)
{
}

IPosition& Enemy::GetPosition()
{
    return m_Position;
}

const IPosition& Enemy::GetPosition() const
{
    return m_Position;
}

EnemyShape Enemy::GetShape() const
{
    return m_Shape;
}

EnemyState Enemy::GetState() const
{
    return m_State;
}

float Enemy::GetSize() const
{
    return m_Size;
}

float Enemy::GetSpeed() const
{
    return m_Speed;
}

void Enemy::Spawn(float x, float y)
{
    m_Position.SetX(x);
    m_Position.SetY(y);
    m_State = EnemyState::Active;
}

void Enemy::Kill()
{
    m_State = EnemyState::Dead;
}

void Enemy::Update(float deltaTime)
{
    if (m_State != EnemyState::Active)
        return;
    
    m_Position.Move(-m_Speed * deltaTime, 0.0f);
}
