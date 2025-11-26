#pragma once

#include "Types/SaveData.h"

class ISaveService {
   public:
    virtual ~ISaveService() = default;

    virtual SaveData LoadProgress() = 0;
    virtual void SaveProgress(const SaveData& data) = 0;

    virtual int GetPoints() const = 0;
    virtual int GetHighPoints() const = 0;
    virtual SaveData GetCurrentData() const = 0;
};
