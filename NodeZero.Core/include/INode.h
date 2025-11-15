#pragma once

#include "Enums/NodeState.h"

class IPosition
{
public:
    virtual ~IPosition() = default;

    virtual float GetX() const = 0;
    virtual float GetY() const = 0;

    virtual void SetX(float x) = 0;
    virtual void SetY(float y) = 0;

    virtual void Move(float deltaX, float deltaY) = 0;
};

class INode
{
public:
    virtual ~INode() = default;

    virtual IPosition& GetPosition() = 0;
    virtual const IPosition& GetPosition() const = 0;
    virtual NodeShape GetShape() const = 0;

    virtual NodeState GetState() const = 0;

    virtual float GetSize() const = 0;

    virtual float GetSpeed() const = 0;

    virtual float GetHP() const = 0;

    virtual float GetMaxHP() const = 0;

    virtual void TakeDamage(float damage) = 0;

    virtual void Spawn(float x, float y) = 0;

    virtual void Kill() = 0;

    virtual void Update(float deltaTime) = 0;
};
