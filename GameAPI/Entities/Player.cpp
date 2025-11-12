#include "Player.h"

Player::Player()
    : m_RotationDegrees(0.0f)
{
}

float Player::GetRotationDegrees() const
{
    return m_RotationDegrees;
}

void Player::SetRotationDegrees(float degrees)
{
    m_RotationDegrees = degrees;
}

void Player::Update(float deltaTime)
{
    // Update logic aici (animatii, efecte, etc.)
}
