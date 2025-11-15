#include "Position.h"

Position::Position(float x, float y)
    : m_X(x), m_Y(y)
{
}

float Position::GetX() const
{
    return m_X;
}

float Position::GetY() const
{
    return m_Y;
}

void Position::SetX(float x)
{
    m_X = x;
}

void Position::SetY(float y)
{
    m_Y = y;
}

void Position::Move(float deltaX, float deltaY)
{
    m_X += deltaX;
    m_Y += deltaY;
}
