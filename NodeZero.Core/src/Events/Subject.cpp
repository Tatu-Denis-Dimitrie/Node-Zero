#include "Events/Subject.h"

void Subject::Attach(std::shared_ptr<IObserver> observer) {
    if (!observer) {
        return;
    }
    m_observers.push_back(observer);
}

void Subject::Detach(std::shared_ptr<IObserver> observer) {
    if (!observer) {
        return;
    }
    m_observers.remove(observer);
}

void Subject::Notify(const std::shared_ptr<IEvent>& event) {
    if (!event) {
        return;
    }

    // Create a copy to avoid issues if observers are modified during notification
    auto observersCopy = m_observers;
    for (const auto& observer : observersCopy) {
        if (observer) {
            observer->Update(event);
        }
    }
}
