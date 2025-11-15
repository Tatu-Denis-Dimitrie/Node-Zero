#pragma once

#include "../Core/IEnemy.h"

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
