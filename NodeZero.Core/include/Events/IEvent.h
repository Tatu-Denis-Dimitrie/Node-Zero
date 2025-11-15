#pragma once
#include <string>
#include <memory>

namespace NodeZero {

/**
 * @brief Base interface for all game events in the Observer pattern
 *
 * Events are immutable data carriers that notify listeners about
 * changes in game state without coupling the sender to receivers.
 */
class IEvent {
public:
    virtual ~IEvent() = default;

    /**
     * @brief Get the type identifier for this event
     * @return String representing the event type (e.g., "NodeDestroyed")
     */
    virtual std::string GetType() const = 0;

    /**
     * @brief Get timestamp when event was created (in seconds)
     */
    virtual float GetTimestamp() const = 0;
};

} // namespace NodeZero
