#pragma once
#include <algorithm>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "IEvent.h"
#include "IObserver.h"
#include "ISubject.h"

class Subject : public ISubject {
   public:
    Subject() = default;
    virtual ~Subject() = default;

    void Attach(std::shared_ptr<IObserver> observer) override;

    void Detach(std::shared_ptr<IObserver> observer) override;

    void Notify(const std::shared_ptr<IEvent>& event) override;

   private:
    std::list<std::shared_ptr<IObserver>> m_observers;
};
