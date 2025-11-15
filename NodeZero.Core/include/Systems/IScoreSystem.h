#pragma once

/// <summary>
/// Interfață pentru sistemul de punctaj
/// </summary>
class IScoreSystem
{
public:
    virtual ~IScoreSystem() = default;

    /// <summary>
    /// Adaugă puncte la scorul curent
    /// </summary>
    /// <param name="points">Numărul de puncte de adăugat</param>
    virtual void AddScore(int points) = 0;

    /// <summary>
    /// Obține scorul curent
    /// </summary>
    /// <returns>Scorul curent</returns>
    virtual int GetScore() const = 0;

    /// <summary>
    /// Resetează scorul la 0
    /// </summary>
    virtual void ResetScore() = 0;

    /// <summary>
    /// Incrementează multiplicatorul de puncte
    /// </summary>
    virtual void IncreaseMultiplier() = 0;

    /// <summary>
    /// Resetează multiplicatorul de puncte la 1
    /// </summary>
    virtual void ResetMultiplier() = 0;

    /// <summary>
    /// Obține multiplicatorul curent de puncte
    /// </summary>
    /// <returns>Multiplicatorul curent</returns>
    virtual int GetMultiplier() const = 0;

    /// <summary>
    /// Obține cel mai mare scor (high score)
    /// </summary>
    /// <returns>High score-ul</returns>
    virtual int GetHighScore() const = 0;

    /// <summary>
    /// Actualizează high score-ul dacă scorul curent este mai mare
    /// </summary>
    virtual void UpdateHighScore() = 0;
};
