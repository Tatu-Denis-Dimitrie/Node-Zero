#pragma once

#include "IPlayer.h"

class IGame
{
public:
    virtual ~IGame() = default;

    virtual void Initialize(float screenWidth, float screenHeight) = 0;
    virtual void Update(float deltaTime) = 0;
    
    virtual IPlayer& GetPlayer() = 0;
    virtual const IPlayer& GetPlayer() const = 0;
};
