#pragma once

#include "raylib.h"

class InputHandler {
   public:
    static Vector2 GetMousePosition();

    static bool IsMouseButtonPressed(int button);

    static bool IsKeyPressed(int key);

    static bool IsKeyDown(int key);
};
