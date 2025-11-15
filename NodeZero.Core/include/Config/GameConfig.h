#pragma once

struct GameConfig
{
    static constexpr float DEFAULT_SCREEN_WIDTH = 1200.0f;
    static constexpr float DEFAULT_SCREEN_HEIGHT = 800.0f;

    static constexpr float PLAYER_SIZE = 30.0f;
    static constexpr float PLAYER_ROTATION_SPEED = 360.0f;

    static constexpr float NODE_MIN_SIZE = 20.0f;
    static constexpr float NODE_MAX_SIZE = 60.0f;
    static constexpr float NODE_MIN_SPEED = 50.0f;
    static constexpr float NODE_MAX_SPEED = 200.0f;

    static constexpr int MAX_NODES_PER_LEVEL = 50;
    static constexpr float NODE_SPAWN_INTERVAL = 2.0f;

    static constexpr int POINTS_PER_NODE_DESTROYED = 100;
    static constexpr int POINTS_MULTIPLIER_MAX = 5;
};
