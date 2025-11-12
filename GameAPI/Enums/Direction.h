#pragma once

// Enum pentru directiile de miscare
enum class Direction
{
    None = 0,
    Up,
    Down,
    Left,
    Right
};

// Enum pentru rotatia obiectului (in grade)
enum class Rotation
{
    North = 0,      // 0 grade (sus)
    East = 90,      // 90 grade (dreapta)
    South = 180,    // 180 grade (jos)
    West = 270      // 270 grade (stanga)
};
