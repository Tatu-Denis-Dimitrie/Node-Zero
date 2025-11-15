#pragma once

/// <summary>
/// Interfață pentru gestionarea nivelurilor
/// </summary>
class ILevel
{
public:
    virtual ~ILevel() = default;

    /// <summary>
    /// Încarcă datele nivelului
    /// </summary>
    virtual void Load() = 0;

    /// <summary>
    /// Descarcă nivelul curent
    /// </summary>
    virtual void Unload() = 0;

    /// <summary>
    /// Actualizează logica nivelului
    /// </summary>
    virtual void Update(float deltaTime) = 0;

    /// <summary>
    /// Verifică dacă nivelul este completat
    /// </summary>
    virtual bool IsCompleted() const = 0;

    /// <summary>
    /// Obține dificultatea nivelului (1-10)
    /// </summary>
    virtual int GetDifficulty() const = 0;
};
