#pragma once

#include "Systems/ICollisionSystem.h"

class CollisionSystem : public ICollisionSystem {
   public:
    bool CheckCircleCollision(float x1, float y1, float radius1,
                              float x2, float y2, float radius2) const override;

    bool CheckRectCollision(float x1, float y1, float width1, float height1,
                            float x2, float y2, float width2, float height2) const override;

    bool CheckPointInCircle(float pointX, float pointY,
                            float circleX, float circleY, float radius) const override;

    float CalculateDistance(float x1, float y1, float x2, float y2) const override;
};
