#pragma once

/// <summary>
/// Enumerare pentru stările principale ale jocului
/// </summary>
enum class GameState
{
    /// <summary>
    /// Jocul este în meniul principal
    /// </summary>
    MainMenu,

    /// <summary>
    /// Jocul este în curs de desfășurare
    /// </summary>
    Playing,

    /// <summary>
    /// Jocul este pus pe pauză
    /// </summary>
    Paused,

    /// <summary>
    /// Jocul s-a terminat (game over)
    /// </summary>
    GameOver,

    /// <summary>
    /// Jocul este în ecranul de setări
    /// </summary>
    Settings
};
