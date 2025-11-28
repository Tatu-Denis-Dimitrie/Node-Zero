#pragma once

#include "Position.h"
#include "../Enums/NodeShape.h"

struct SpawnInfo {
    Position position;
    NodeShape shape;
    float directionX;
    float directionY;
};
