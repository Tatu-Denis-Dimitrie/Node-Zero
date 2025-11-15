#pragma once

#include <vector>

class INode;
class ILevel;

namespace NodeZero {
    class EventManager;
}

class IGame
{
public:
    virtual ~IGame() = default;

    virtual void Initialize(float screenWidth, float screenHeight) = 0;

    virtual void Update(float deltaTime) = 0;

    virtual float GetScreenWidth() const = 0;

    virtual float GetScreenHeight() const = 0;

    virtual const std::vector<INode*>& GetNodes() const = 0;

    virtual void SpawnNode(float x, float y) = 0;

    /**
     * @brief Get the event manager for subscribing to game events
     * @return Reference to the game's event manager
     */
    virtual NodeZero::EventManager& GetEventManager() = 0;
};
