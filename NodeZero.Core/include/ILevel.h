#pragma once

class ILevel {
   public:
    virtual ~ILevel() = default;

    virtual void Load() = 0;

    virtual void Unload() = 0;

    virtual void Update(float deltaTime) = 0;

    virtual bool IsCompleted() const = 0;

    virtual int GetDifficulty() const = 0;
};
