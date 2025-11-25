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
    char appDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        savePath = std::string(appDataPath) + "\\NodeZero\\";

        CreateDirectoryA(savePath.c_str(), NULL);
    } else {
        savePath = "./";
    }
#else
    const char* homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        homeDir = getpwuid(getuid())->pw_dir;
    }

    if (homeDir != nullptr) {
        savePath = std::string(homeDir) + "/.local/share/NodeZero/";

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
    file << "  \"highPoints\": " << data.highPoints << ",\n";
    file << "  \"points\": " << data.points << ",\n";
    file << "  \"gamesPlayed\": " << data.gamesPlayed << ",\n";
    file << "  \"totalNodesDestroyed\": " << data.totalNodesDestroyed << ",\n";
    file << "  \"currentLevel\": " << data.currentLevel << ",\n";
    file << "  \"maxHealth\": " << data.maxHealth << ",\n";
    file << "  \"regenRate\": " << data.regenRate << ",\n";
    file << "  \"damageZoneSize\": " << data.damageZoneSize << ",\n";
    file << "  \"damagePerTick\": " << data.damagePerTick << "\n";
    file << "}\n";

    file.close();
    return true;
}

SaveData SaveSystem::LoadProgress() {
    SaveData data;
    std::string filePath = GetSavePath();
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
        key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
        value.erase(std::remove(value.begin(), value.end(), ','), value.end());
        value.erase(std::remove(value.begin(), value.end(), ' '), value.end());

        if (key == "highPoints") {
            data.highPoints = std::stoi(value);
        } else if (key == "points") {
            data.points = std::stoi(value);
        } else if (key == "gamesPlayed") {
            data.gamesPlayed = std::stoi(value);
        } else if (key == "totalNodesDestroyed") {
            data.totalNodesDestroyed = std::stoi(value);
        } else if (key == "currentLevel") {
            data.currentLevel = std::stoi(value);
        } else if (key == "maxHealth") {
            data.maxHealth = std::stof(value);
        } else if (key == "regenRate") {
            data.regenRate = std::stof(value);
        } else if (key == "damageZoneSize") {
            data.damageZoneSize = std::stof(value);
        } else if (key == "damagePerTick") {
            data.damagePerTick = std::stof(value);
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
