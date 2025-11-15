#pragma once
#include <memory>

class IEvent;

class IEventListener {
   public:
    virtual ~IEventListener() = default;

    virtual void OnEvent(const std::shared_ptr<IEvent>& event) = 0;
};
