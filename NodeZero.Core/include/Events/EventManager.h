#pragma once
#include "IEvent.h"
#include "IEventListener.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

namespace NodeZero {

/**
 * @brief Central event dispatcher implementing the Observer pattern
 *
 * Manages event subscriptions and dispatches events to registered listeners.
 * Provides decoupling between event publishers (game logic) and subscribers (UI, audio, etc.)
 */
class EventManager {
public:
    EventManager() = default;
    ~EventManager() = default;

    // Non-copyable
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    /**
     * @brief Subscribe a listener to a specific event type
     * @param eventType The type of event to listen for (e.g., "NodeDestroyed")
     * @param listener The listener to notify when events of this type occur
     */
    void Subscribe(const std::string& eventType, std::shared_ptr<IEventListener> listener);

    /**
     * @brief Unsubscribe a listener from a specific event type
     * @param eventType The type of event to stop listening for
     * @param listener The listener to remove
     */
    void Unsubscribe(const std::string& eventType, std::shared_ptr<IEventListener> listener);

    /**
     * @brief Publish an event to all subscribed listeners
     * @param event The event to publish
     */
    void Publish(const std::shared_ptr<IEvent>& event);

    /**
     * @brief Clear all event subscriptions
     */
    void Clear();

private:
    // Map: EventType -> List of Listeners
    std::unordered_map<std::string, std::vector<std::shared_ptr<IEventListener>>> m_listeners;
};

} // namespace NodeZero
