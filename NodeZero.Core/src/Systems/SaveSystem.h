#pragma once

#include <string>

#include "Types/SaveData.h"

class SaveSystem {
   public:
    static bool SaveProgress(const SaveData& data);
    static SaveData LoadProgress();
    static bool HasSaveFile();
    static std::string GetSavePath();

   private:
    static constexpr const char* SAVE_FILE_NAME = "save.dat";
};
