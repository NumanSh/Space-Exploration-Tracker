#include "ConfigManager.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

ConfigManager::ConfigManager(const std::string& config_path) : config_path_(config_path) {}

bool ConfigManager::saveSettings(const UserSettings& settings) {
    try {
        std::ofstream file(config_path_);
        if (file.is_open()) {
            file << settings.toJson().dump(4);
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error saving settings: " << e.what() << std::endl;
    }
    return false;
}

bool ConfigManager::loadSettings(UserSettings& out_settings) {
    try {
        if (!fs::exists(config_path_)) {
            return false;
        }

        std::ifstream file(config_path_);
        if (file.is_open()) {
            nlohmann::json j;
            file >> j;
            out_settings.fromJson(j);
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading settings: " << e.what() << std::endl;
    }
    return false;
}

void ConfigManager::ensureLogDirectory(const std::string& dir_name) {
    if (!fs::exists(dir_name)) {
        if (fs::create_directory(dir_name)) {
            std::cout << "Created directory: " << dir_name << std::endl;
        }
    }
}

void ConfigManager::writeLog(const std::string& message) {
    ensureLogDirectory("logs");
    
    std::string log_file = "logs/app_log.txt";
    std::ofstream file(log_file, std::ios::app);
    if (file.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        
        file << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << " : " << message << std::endl;
    }
}
