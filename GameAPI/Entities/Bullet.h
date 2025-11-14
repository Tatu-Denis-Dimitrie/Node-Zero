#pragma once

class Bullet
{
private:
    float m_X;
    float m_Y;
    float m_VelocityX;
    float m_VelocityY;
    float m_RotationDegrees;
    float m_Speed;

public:
    Bullet(float x, float y, float angleDegrees, float speed = 400.0f);

    void Update(float deltaTime);

    float GetX() const;
    float GetY() const;
    float GetRotationDegrees() const;

    bool IsOutOfBounds(float screenWidth, float screenHeight) const;
};
