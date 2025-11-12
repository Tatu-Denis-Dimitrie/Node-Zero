#pragma once

#include "../Enums/Direction.h"

class IPlayer
{
public:
    virtual ~IPlayer() = default;

    virtual Rotation GetRotation() const = 0;
    virtual void SetRotation(Rotation rotation) = 0;
    
    virtual void Rotate(Direction direction) = 0;
    virtual void Update(float deltaTime) = 0;
};
