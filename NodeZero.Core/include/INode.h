#pragma once

#include "Enums/NodeState.h"

/// <summary>
/// Interfață de bază pentru poziționare
/// </summary>
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

/// <summary>
/// Interfață pentru noduri (entități inamice)
/// </summary>
class INode
{
public:
    virtual ~INode() = default;

    /// <summary>
    /// Obține poziția nodului
    /// </summary>
    virtual IPosition& GetPosition() = 0;
    virtual const IPosition& GetPosition() const = 0;

    /// <summary>
    /// Obține forma nodului
    /// </summary>
    virtual NodeShape GetShape() const = 0;

    /// <summary>
    /// Obține starea nodului
    /// </summary>
    virtual NodeState GetState() const = 0;

    /// <summary>
    /// Obține dimensiunea nodului
    /// </summary>
    virtual float GetSize() const = 0;

    /// <summary>
    /// Obține viteza nodului
    /// </summary>
    virtual float GetSpeed() const = 0;

    /// <summary>
    /// Obține HP-ul curent al nodului
    /// </summary>
    virtual float GetHP() const = 0;

    /// <summary>
    /// Obține HP-ul maxim al nodului
    /// </summary>
    virtual float GetMaxHP() const = 0;

    /// <summary>
    /// Aplicare damage nodului
    /// </summary>
    virtual void TakeDamage(float damage) = 0;

    /// <summary>
    /// Spawn-uiește nodul la coordonatele specificate
    /// </summary>
    virtual void Spawn(float x, float y) = 0;

    /// <summary>
    /// Distruge nodul
    /// </summary>
    virtual void Kill() = 0;

    /// <summary>
    /// Actualizează starea nodului
    /// </summary>
    virtual void Update(float deltaTime) = 0;
};
