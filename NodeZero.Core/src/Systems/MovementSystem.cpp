#include "../../include/Systems/IMovementSystem.h"
#include <cmath>

class MovementSystem : public IMovementSystem
{
public:
    void Move(float& x, float& y, Direction direction, float speed, float deltaTime) override
    {
        float distance = speed * deltaTime;

        switch (direction)
        {
        case Direction::Up:
            y -= distance;
            break;
        case Direction::Down:
            y += distance;
            break;
        case Direction::Left:
            x -= distance;
            break;
        case Direction::Right:
            x += distance;
            break;
        case Direction::None:
        default:
            break;
        }
    }

    void MoveTowards(float& x, float& y, float targetX, float targetY, float speed, float deltaTime) override
    {
        float dx = targetX - x;
        float dy = targetY - y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 0.1f)
            return;

        float moveDistance = speed * deltaTime;
        if (moveDistance > distance)
            moveDistance = distance;

        float dirX = dx / distance;
        float dirY = dy / distance;

        x += dirX * moveDistance;
        y += dirY * moveDistance;
    }

    void CalculateVelocity(float angleDegrees, float speed, float& velocityX, float& velocityY) override
    {
        const float PI = 3.14159265358979323846f;
        float angleRadians = angleDegrees * PI / 180.0f;

        velocityX = std::cos(angleRadians) * speed;
        velocityY = std::sin(angleRadians) * speed;
    }
};
