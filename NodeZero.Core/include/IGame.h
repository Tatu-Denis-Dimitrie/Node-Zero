#pragma once

#include <vector>

class INode;
class ILevel;

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
};
