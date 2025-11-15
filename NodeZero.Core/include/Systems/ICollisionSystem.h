#pragma once

/// <summary>
/// Interfață pentru sistemul de coliziuni
/// </summary>
class ICollisionSystem
{
public:
    virtual ~ICollisionSystem() = default;

    /// <summary>
    /// Verifică coliziunea între două cercuri
    /// </summary>
    /// <param name="x1">Coordonata X a primului cerc</param>
    /// <param name="y1">Coordonata Y a primului cerc</param>
    /// <param name="radius1">Raza primului cerc</param>
    /// <param name="x2">Coordonata X a celui de-al doilea cerc</param>
    /// <param name="y2">Coordonata Y a celui de-al doilea cerc</param>
    /// <param name="radius2">Raza celui de-al doilea cerc</param>
    /// <returns>True dacă cercurile se ciocnesc, false altfel</returns>
    virtual bool CheckCircleCollision(float x1, float y1, float radius1,
                                      float x2, float y2, float radius2) const = 0;

    /// <summary>
    /// Verifică coliziunea între două dreptunghiuri
    /// </summary>
    /// <param name="x1">Coordonata X a primului dreptunghi</param>
    /// <param name="y1">Coordonata Y a primului dreptunghi</param>
    /// <param name="width1">Lățimea primului dreptunghi</param>
    /// <param name="height1">Înălțimea primului dreptunghi</param>
    /// <param name="x2">Coordonata X a celui de-al doilea dreptunghi</param>
    /// <param name="y2">Coordonata Y a celui de-al doilea dreptunghi</param>
    /// <param name="width2">Lățimea celui de-al doilea dreptunghi</param>
    /// <param name="height2">Înălțimea celui de-al doilea dreptunghi</param>
    /// <returns>True dacă dreptunghiurile se ciocnesc, false altfel</returns>
    virtual bool CheckRectCollision(float x1, float y1, float width1, float height1,
                                    float x2, float y2, float width2, float height2) const = 0;

    /// <summary>
    /// Verifică dacă un punct se află într-un cerc
    /// </summary>
    /// <param name="pointX">Coordonata X a punctului</param>
    /// <param name="pointY">Coordonata Y a punctului</param>
    /// <param name="circleX">Coordonata X a centrului cercului</param>
    /// <param name="circleY">Coordonata Y a centrului cercului</param>
    /// <param name="radius">Raza cercului</param>
    /// <returns>True dacă punctul se află în cerc, false altfel</returns>
    virtual bool CheckPointInCircle(float pointX, float pointY,
                                    float circleX, float circleY, float radius) const = 0;

    /// <summary>
    /// Calculează distanța între două puncte
    /// </summary>
    /// <param name="x1">Coordonata X a primului punct</param>
    /// <param name="y1">Coordonata Y a primului punct</param>
    /// <param name="x2">Coordonata X a celui de-al doilea punct</param>
    /// <param name="y2">Coordonata Y a celui de-al doilea punct</param>
    /// <returns>Distanța între cele două puncte</returns>
    virtual float CalculateDistance(float x1, float y1, float x2, float y2) const = 0;
};
