#pragma once

class IAppScreen {
   public:
    virtual ~IAppScreen() = default;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
};
