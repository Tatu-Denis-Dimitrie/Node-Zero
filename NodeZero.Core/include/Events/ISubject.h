#pragma once
#include <memory>

class IObserver;
class IEvent;

class ISubject {
   public:
    virtual ~ISubject() = default;
    virtual void Attach(std::shared_ptr<IObserver> observer) = 0;
    virtual void Detach(std::shared_ptr<IObserver> observer) = 0;
    virtual void Notify(const std::shared_ptr<IEvent>& event) = 0;
};
