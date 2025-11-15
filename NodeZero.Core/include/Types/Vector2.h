#pragma once

namespace NodeZero {

/**
 * @brief Simple 2D vector structure
 *
 * Platform-agnostic alternative to raylib's Vector2,
 * keeping Core independent from rendering libraries.
 */
struct Vector2 {
    float x;
    float y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

} // namespace NodeZero
