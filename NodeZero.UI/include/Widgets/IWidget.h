#pragma once

#include "raylib.h"

class IWidget
{
public:
    virtual ~IWidget() = default;

    virtual void Draw() = 0;

    virtual void Update() = 0;

    virtual bool IsHovered() const = 0;

    virtual bool IsActive() const = 0;

    virtual void SetActive(bool active) = 0;
};
