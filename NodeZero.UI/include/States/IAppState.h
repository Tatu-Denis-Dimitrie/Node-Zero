#pragma once

class IAppState {
   public:
    virtual ~IAppState() = default;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
};
