#pragma once
#include <memory>

class IEvent;

class IObserver {
   public:
    virtual ~IObserver() = default;

    virtual void OnEvent(const std::shared_ptr<IEvent>& event) = 0;
};
