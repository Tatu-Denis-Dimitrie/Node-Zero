#pragma once

#include "../Core/IPlayer.h"

// Implementare concreta pentru jucator
class Player : public IPlayer
{
private:
    float m_RotationDegrees;

public:
    Player();

    float GetRotationDegrees() const override;
    void SetRotationDegrees(float degrees) override;
    
    void Update(float deltaTime) override;
};
