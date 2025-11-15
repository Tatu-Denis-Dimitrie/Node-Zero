#pragma once

#include "../include/INode.h"

/// <summary>
/// Implementare pentru poziție în spațiul 2D
/// </summary>
class Position : public IPosition
{
private:
    float m_X;
    float m_Y;

public:
    Position(float x = 0.0f, float y = 0.0f);

    float GetX() const override;
    float GetY() const override;

    void SetX(float x) override;
    void SetY(float y) override;

    void Move(float deltaX, float deltaY) override;
};
