#pragma once
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IEvent.h"
#include "IObserver.h"

class Subject {
   public:
    Subject() = default;
    ~Subject() = default;

    Subject(const Subject&) = delete;
    Subject& operator=(const Subject&) = delete;

    void Attach(const std::string& eventType, std::shared_ptr<IObserver> observer);

    void Detach(const std::string& eventType, std::shared_ptr<IObserver> observer);

    void Notify(const std::shared_ptr<IEvent>& event);

    void Clear();

   private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<IObserver>>> m_observers;
};
