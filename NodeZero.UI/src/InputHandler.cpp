#include "InputHandler.h"

#include <cmath>

Vector2 InputHandler::GetMousePosition() {
    return ::GetMousePosition();
}

bool InputHandler::IsMouseButtonPressed(int button) {
    return ::IsMouseButtonPressed(button);
}

bool InputHandler::IsKeyPressed(int key) {
    return ::IsKeyPressed(key);
}

bool InputHandler::IsKeyDown(int key) {
    return ::IsKeyDown(key);
}
