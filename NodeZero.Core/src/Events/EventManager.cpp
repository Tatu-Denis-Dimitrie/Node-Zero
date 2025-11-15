#include "Events/EventManager.h"

namespace NodeZero {

void EventManager::Subscribe(const std::string& eventType, std::shared_ptr<IEventListener> listener) {
    if (!listener) {
        return; // Ignore null listeners
    }

    m_listeners[eventType].push_back(listener);
}

void EventManager::Unsubscribe(const std::string& eventType, std::shared_ptr<IEventListener> listener) {
    if (!listener) {
        return;
    }

    auto it = m_listeners.find(eventType);
    if (it != m_listeners.end()) {
        auto& listeners = it->second;
        listeners.erase(
            std::remove(listeners.begin(), listeners.end(), listener),
            listeners.end()
        );

        // Remove empty entries to keep map clean
        if (listeners.empty()) {
            m_listeners.erase(it);
        }
    }
}

void EventManager::Publish(const std::shared_ptr<IEvent>& event) {
    if (!event) {
        return;
    }

    const std::string eventType = event->GetType();
    auto it = m_listeners.find(eventType);

    if (it != m_listeners.end()) {
        // Create a copy of listeners to avoid issues if a listener modifies subscriptions
        auto listenersCopy = it->second;

        for (const auto& listener : listenersCopy) {
            if (listener) {
                listener->OnEvent(event);
            }
        }
    }
}

void EventManager::Clear() {
    m_listeners.clear();
}

} // namespace NodeZero
