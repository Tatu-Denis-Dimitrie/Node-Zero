#pragma once

#include "../Enums/EnemyTypes.h"

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

class IEnemy
{
public:
    virtual ~IEnemy() = default;

    virtual IPosition& GetPosition() = 0;
    virtual const IPosition& GetPosition() const = 0;
    
    virtual EnemyShape GetShape() const = 0;
    virtual EnemyState GetState() const = 0;
    
    virtual float GetSize() const = 0;
    virtual float GetSpeed() const = 0;
    
    virtual void Spawn(float x, float y) = 0;
    virtual void Kill() = 0;
    virtual void Update(float deltaTime) = 0;
};
