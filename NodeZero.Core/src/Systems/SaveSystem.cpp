#include "../../include/Systems/SaveSystem.h"

#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <shlobj.h>
#include <windows.h>
#else
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

std::string SaveSystem::GetSavePath() {
    std::string savePath;

#ifdef _WIN32
    // Windows: Use %APPDATA%/NodeZero/
    char appDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        savePath = std::string(appDataPath) + "\\NodeZero\\";

        // Create directory if it doesn't exist
        CreateDirectoryA(savePath.c_str(), NULL);
    } else {
        // Fallback to current directory
        savePath = "./";
    }
#else
    // Linux/Mac: Use ~/.local/share/NodeZero/
    const char* homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        homeDir = getpwuid(getuid())->pw_dir;
    }

    if (homeDir != nullptr) {
        savePath = std::string(homeDir) + "/.local/share/NodeZero/";

        // Create directory if it doesn't exist
        std::string mkdirCmd = "mkdir -p " + savePath;
        system(mkdirCmd.c_str());
    } else {
        savePath = "./";
    }
#endif

    return savePath + SAVE_FILE_NAME;
}

bool SaveSystem::SaveProgress(const SaveData& data) {
    std::string filePath = GetSavePath();
    std::ofstream file(filePath);

    if (!file.is_open()) {
        return false;
    }

    file << "{\n";
    file << "  \"highScore\": " << data.highScore << ",\n";
    file << "  \"gamesPlayed\": " << data.gamesPlayed << ",\n";
    file << "  \"totalNodesDestroyed\": " << data.totalNodesDestroyed << ",\n";
    file << "  \"maxHealth\": " << data.maxHealth << "\n";
    file << "}\n";

    file.close();
    return true;
}

SaveData SaveSystem::LoadProgress() {
    SaveData data;
    std::string filePath = GetSavePath();
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return data;  // Return default values
    }

    std::string line;
    while (std::getline(file, line)) {
        // Parse simple JSON format
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        // Remove quotes, spaces, commas
        key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
        key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
        value.erase(std::remove(value.begin(), value.end(), ','), value.end());
        value.erase(std::remove(value.begin(), value.end(), ' '), value.end());

        if (key == "highScore") {
            data.highScore = std::stoi(value);
        } else if (key == "gamesPlayed") {
            data.gamesPlayed = std::stoi(value);
        } else if (key == "totalNodesDestroyed") {
            data.totalNodesDestroyed = std::stoi(value);
        } else if (key == "maxHealth") {
            data.maxHealth = std::stof(value);
        }
    }

    file.close();
    return data;
}

bool SaveSystem::HasSaveFile() {
    std::string filePath = GetSavePath();
    std::ifstream file(filePath);
    return file.good();
}
