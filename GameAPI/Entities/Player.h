#pragma once

#include "../Core/IPlayer.h"

// Implementare concreta pentru jucator
class Player : public IPlayer
{
private:
    Rotation m_Rotation;

public:
    Player();

    Rotation GetRotation() const override;
    void SetRotation(Rotation rotation) override;
    
    void Rotate(Direction direction) override;
    void Update(float deltaTime) override;
};
