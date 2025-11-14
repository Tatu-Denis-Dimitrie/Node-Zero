#include "Bullet.h"
#include <cmath>

#ifndef DEG2RAD
#define DEG2RAD (3.14159265358979323846f / 180.0f)
#endif

Bullet::Bullet(float x, float y, float angleDegrees, float speed)
    : m_X(x), m_Y(y), m_RotationDegrees(angleDegrees), m_Speed(speed)
{
    float angleRad = angleDegrees * DEG2RAD;
    m_VelocityX = sinf(angleRad) * m_Speed;
    m_VelocityY = -cosf(angleRad) * m_Speed;
}

void Bullet::Update(float deltaTime)
{
    m_X += m_VelocityX * deltaTime;
    m_Y += m_VelocityY * deltaTime;
}

float Bullet::GetX() const
{
    return m_X;
}

float Bullet::GetY() const
{
    return m_Y;
}

float Bullet::GetRotationDegrees() const
{
    return m_RotationDegrees;
}

bool Bullet::IsOutOfBounds(float screenWidth, float screenHeight) const
{
    return m_X < -50.0f || m_X > screenWidth + 50.0f || 
           m_Y < -50.0f || m_Y > screenHeight + 50.0f;
}
