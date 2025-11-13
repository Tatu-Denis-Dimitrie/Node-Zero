#pragma once

#include "../Core/IPlayer.h"

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
