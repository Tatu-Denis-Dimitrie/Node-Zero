#include "Events/Subject.h"

void Subject::Attach(const std::string& eventType, std::shared_ptr<IObserver> observer) {
    if (!observer) {
        return;
    }

    m_observers[eventType].push_back(observer);
}

void Subject::Detach(const std::string& eventType, std::shared_ptr<IObserver> observer) {
    if (!observer) {
        return;
    }

    auto it = m_observers.find(eventType);
    if (it != m_observers.end()) {
        auto& observers = it->second;
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end());

        if (observers.empty()) {
            m_observers.erase(it);
        }
    }
}

void Subject::Notify(const std::shared_ptr<IEvent>& event) {
    if (!event) {
        return;
    }

    const std::string eventType = event->GetType();
    auto it = m_observers.find(eventType);

    if (it != m_observers.end()) {
        auto observersCopy = it->second;

        for (const auto& observer : observersCopy) {
            if (observer) {
                observer->OnEvent(event);
            }
        }
    }
}

void Subject::Clear() {
    m_observers.clear();
}
