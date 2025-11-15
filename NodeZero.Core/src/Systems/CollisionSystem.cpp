#include "../../include/Systems/ICollisionSystem.h"
#include <cmath>

/// <summary>
/// Implementare concretă a sistemului de coliziuni
/// </summary>
class CollisionSystem : public ICollisionSystem
{
public:
    bool CheckCircleCollision(float x1, float y1, float radius1,
                              float x2, float y2, float radius2) const override
    {
        float distance = CalculateDistance(x1, y1, x2, y2);
        return distance <= (radius1 + radius2);
    }

    bool CheckRectCollision(float x1, float y1, float width1, float height1,
                            float x2, float y2, float width2, float height2) const override
    {
        return (x1 < x2 + width2 &&
                x1 + width1 > x2 &&
                y1 < y2 + height2 &&
                y1 + height1 > y2);
    }

    bool CheckPointInCircle(float pointX, float pointY,
                            float circleX, float circleY, float radius) const override
    {
        float distance = CalculateDistance(pointX, pointY, circleX, circleY);
        return distance <= radius;
    }

    float CalculateDistance(float x1, float y1, float x2, float y2) const override
    {
        float dx = x2 - x1;
        float dy = y2 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }
};
