#pragma once

#include "raylib.h"

/// <summary>
/// Interfață de bază pentru toate widget-urile UI
/// </summary>
class IWidget
{
public:
    virtual ~IWidget() = default;

    /// <summary>
    /// Desenează widget-ul
    /// </summary>
    virtual void Draw() = 0;

    /// <summary>
    /// Actualizează starea widget-ului
    /// </summary>
    virtual void Update() = 0;

    /// <summary>
    /// Verifică dacă mouse-ul este peste widget
    /// </summary>
    virtual bool IsHovered() const = 0;

    /// <summary>
    /// Verifică dacă widget-ul este activ
    /// </summary>
    virtual bool IsActive() const = 0;

    /// <summary>
    /// Setează dacă widget-ul este activ
    /// </summary>
    virtual void SetActive(bool active) = 0;
};
