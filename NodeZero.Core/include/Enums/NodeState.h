#pragma once

/// <summary>
/// Forme disponibile pentru noduri
/// </summary>
enum class NodeShape
{
    /// <summary>
    /// Formă circulară
    /// </summary>
    Circle = 0,

    /// <summary>
    /// Formă pătrată
    /// </summary>
    Square,

    /// <summary>
    /// Formă triunghiulară
    /// </summary>
    Triangle,

    /// <summary>
    /// Formă pentagonală
    /// </summary>
    Pentagon,

    /// <summary>
    /// Formă hexagonală
    /// </summary>
    Hexagon
};

/// <summary>
/// Stări posibile ale unui nod
/// </summary>
enum class NodeState
{
    /// <summary>
    /// Nodul este inactiv (nu a fost spawn-uit încă)
    /// </summary>
    Inactive = 0,

    /// <summary>
    /// Nodul este activ și în mișcare
    /// </summary>
    Active,

    /// <summary>
    /// Nodul a fost distrus
    /// </summary>
    Dead
};
