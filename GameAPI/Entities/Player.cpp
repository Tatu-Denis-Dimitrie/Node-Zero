#include "Player.h"

Player::Player()
    : m_Rotation(Rotation::North)
{
}

Rotation Player::GetRotation() const
{
    return m_Rotation;
}

void Player::SetRotation(Rotation rotation)
{
    m_Rotation = rotation;
}

void Player::Rotate(Direction direction)
{
    switch (direction)
    {
    case Direction::Up:
        m_Rotation = Rotation::North;
        break;
    case Direction::Down:
        m_Rotation = Rotation::South;
        break;
    case Direction::Left:
        m_Rotation = Rotation::West;
        break;
    case Direction::Right:
        m_Rotation = Rotation::East;
        break;
    case Direction::None:
        break;
    }
}

void Player::Update(float deltaTime)
{
}
