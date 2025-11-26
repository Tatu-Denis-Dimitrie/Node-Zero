#pragma once

#include "Services/ISaveService.h"
#include "Types/SaveData.h"

class SaveService : public ISaveService {
   private:
    SaveData m_CurrentData;

   public:
    SaveService();
    ~SaveService() override = default;

    void Initialize();

    SaveData LoadProgress() override;
    void SaveProgress(const SaveData& data) override;

    int GetPoints() const override;
    int GetHighPoints() const override;
    SaveData GetCurrentData() const override;
};
