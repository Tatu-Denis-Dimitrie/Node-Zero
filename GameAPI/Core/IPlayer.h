#pragma once

// Interfata pentru jucator (caracterul principal - sageata)
class IPlayer
{
public:
    virtual ~IPlayer() = default;

    virtual float GetRotationDegrees() const = 0;
    virtual void SetRotationDegrees(float degrees) = 0;
    
    virtual void Update(float deltaTime) = 0;
};
