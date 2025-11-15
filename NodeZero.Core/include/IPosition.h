#pragma once

/**
 * @brief Interface for 2D position management
 *
 * Provides abstraction for positioning entities in 2D space.
 * Decoupled from INode to follow Single Responsibility Principle.
 */
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
