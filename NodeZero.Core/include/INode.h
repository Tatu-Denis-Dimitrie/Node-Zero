#pragma once

#include "Enums/NodeShape.h"
#include "Enums/NodeState.h"
#include "Types/Position.h"

class INode {
   public:
    virtual ~INode() = default;

    virtual Position& GetPosition() = 0;
    virtual const Position& GetPosition() const = 0;
    virtual NodeShape GetShape() const = 0;
    virtual NodeState GetState() const = 0;

    virtual float GetSize() const = 0;
    virtual float GetSpeed() const = 0;
    virtual float GetRotation() const = 0;
    virtual float GetHP() const = 0;
    virtual float GetMaxHP() const = 0;

    virtual void TakeDamage(float damage) = 0;
    virtual void Spawn(float x, float y) = 0;
    virtual void SetDirection(float dirX, float dirY) = 0;
    virtual void Kill() = 0;

    virtual void Update(float deltaTime) = 0;
};
