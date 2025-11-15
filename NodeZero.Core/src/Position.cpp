#include "../include/INode.h"

/// <summary>
/// Implementare concretă pentru poziționare
/// </summary>
class Position : public IPosition
{
private:
    float m_X;
    float m_Y;

public:
    Position(float x = 0.0f, float y = 0.0f)
        : m_X(x), m_Y(y)
    {
    }

    float GetX() const override
    {
        return m_X;
    }

    float GetY() const override
    {
        return m_Y;
    }

    void SetX(float x) override
    {
        m_X = x;
    }

    void SetY(float y) override
    {
        m_Y = y;
    }

    void Move(float deltaX, float deltaY) override
    {
        m_X += deltaX;
        m_Y += deltaY;
    }
};
