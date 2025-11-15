#pragma once
#include <memory>
#include <string>

class IEvent {
   public:
    virtual ~IEvent() = default;

    virtual std::string GetType() const = 0;

    virtual float GetTimestamp() const = 0;
};
