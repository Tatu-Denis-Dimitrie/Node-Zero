#pragma once

class ICollisionSystem
{
public:
    virtual ~ICollisionSystem() = default;

    virtual bool CheckCircleCollision(float x1, float y1, float radius1,
                                      float x2, float y2, float radius2) const = 0;

    virtual bool CheckRectCollision(float x1, float y1, float width1, float height1,
                                    float x2, float y2, float width2, float height2) const = 0;

    virtual bool CheckPointInCircle(float pointX, float pointY,
                                    float circleX, float circleY, float radius) const = 0;

    virtual float CalculateDistance(float x1, float y1, float x2, float y2) const = 0;
};
