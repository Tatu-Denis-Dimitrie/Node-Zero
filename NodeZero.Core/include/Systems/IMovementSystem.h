#pragma once

#include "../Enums/Direction.h"

/// <summary>
/// Interfață pentru sistemul de mișcare
/// </summary>
class IMovementSystem
{
public:
    virtual ~IMovementSystem() = default;

    /// <summary>
    /// Mișcă o entitate în direcția specificată
    /// </summary>
    /// <param name="x">Referință la coordonata X</param>
    /// <param name="y">Referință la coordonata Y</param>
    /// <param name="direction">Direcția de mișcare</param>
    /// <param name="speed">Viteza de mișcare</param>
    /// <param name="deltaTime">Timpul scurs de la ultimul frame</param>
    virtual void Move(float& x, float& y, Direction direction, float speed, float deltaTime) = 0;

    /// <summary>
    /// Mișcă o entitate spre o coordonată țintă
    /// </summary>
    /// <param name="x">Referință la coordonata X</param>
    /// <param name="y">Referință la coordonata Y</param>
    /// <param name="targetX">Coordonata X țintă</param>
    /// <param name="targetY">Coordonata Y țintă</param>
    /// <param name="speed">Viteza de mișcare</param>
    /// <param name="deltaTime">Timpul scurs de la ultimul frame</param>
    virtual void MoveTowards(float& x, float& y, float targetX, float targetY, float speed, float deltaTime) = 0;

    /// <summary>
    /// Calculează viteza pe axele X și Y pe baza unui unghi
    /// </summary>
    /// <param name="angleDegrees">Unghiul în grade</param>
    /// <param name="speed">Viteza totală</param>
    /// <param name="velocityX">Output: viteza pe axa X</param>
    /// <param name="velocityY">Output: viteza pe axa Y</param>
    virtual void CalculateVelocity(float angleDegrees, float speed, float& velocityX, float& velocityY) = 0;
};
