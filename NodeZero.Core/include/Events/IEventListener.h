#pragma once
#include <memory>

namespace NodeZero {

class IEvent;

/**
 * @brief Interface for observers in the Observer pattern
 *
 * Components that want to react to game events should implement
 * this interface and register with the EventManager.
 */
class IEventListener {
public:
    virtual ~IEventListener() = default;

    /**
     * @brief Called when an event this listener is subscribed to occurs
     * @param event The event that occurred
     */
    virtual void OnEvent(const std::shared_ptr<IEvent>& event) = 0;
};

} // namespace NodeZero
