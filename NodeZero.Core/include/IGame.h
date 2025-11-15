#pragma once

#include <vector>

// Forward declarations
class INode;
class ILevel;

/// <summary>
/// Interfață pentru logica principală a jocului
/// </summary>
class IGame
{
public:
    virtual ~IGame() = default;

    /// <summary>
    /// Inițializează jocul cu dimensiunile ecranului
    /// </summary>
    virtual void Initialize(float screenWidth, float screenHeight) = 0;

    /// <summary>
    /// Actualizează starea jocului
    /// </summary>
    virtual void Update(float deltaTime) = 0;

    /// <summary>
    /// Obține dimensiunea lățimii ecranului
    /// </summary>
    virtual float GetScreenWidth() const = 0;

    /// <summary>
    /// Obține dimensiunea înălțimii ecranului
    /// </summary>
    virtual float GetScreenHeight() const = 0;

    /// <summary>
    /// Obține lista de noduri active în joc
    /// </summary>
    virtual const std::vector<INode*>& GetNodes() const = 0;

    /// <summary>
    /// Spawn-uiește un nou nod în joc
    /// </summary>
    virtual void SpawnNode(float x, float y) = 0;
};
