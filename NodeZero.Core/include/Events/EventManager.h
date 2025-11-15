#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IEvent.h"
#include "IEventListener.h"

class EventManager {
   public:
    EventManager() = default;
    ~EventManager() = default;

    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    void Subscribe(const std::string& eventType, std::shared_ptr<IEventListener> listener);

    void Unsubscribe(const std::string& eventType, std::shared_ptr<IEventListener> listener);

    void Publish(const std::shared_ptr<IEvent>& event);

    void Clear();

   private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<IEventListener>>> m_listeners;
};
