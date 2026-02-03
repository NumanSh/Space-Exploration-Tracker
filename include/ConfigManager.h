#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct UserSettings {
    std::string user_name = "User";
    double latitude = 0.0;
    double longitude = 0.0;
    std::string location_name = "Unknown";
    std::vector<std::string> watchlist;
    int refresh_interval = 60; // Default to 60 seconds

    // Convert to JSON
    nlohmann::json toJson() const {
        return {
            {"user_name", user_name},
            {"latitude", latitude},
            {"longitude", longitude},
            {"location_name", location_name},
            {"watchlist", watchlist},
            {"refresh_interval", refresh_interval}
        };
    }

    // Load from JSON
    void fromJson(const nlohmann::json& j) {
        user_name = j.value("user_name", "User");
        latitude = j.value("latitude", 0.0);
        longitude = j.value("longitude", 0.0);
        location_name = j.value("location_name", "Unknown");
        watchlist = j.value("watchlist", std::vector<std::string>());
        refresh_interval = j.value("refresh_interval", 60);
    }
};

class ConfigManager {
public:
    ConfigManager(const std::string& config_path = "config.json");

    bool saveSettings(const UserSettings& settings);
    bool loadSettings(UserSettings& out_settings);
    
    // Demonstrate std::filesystem usage
    void ensureLogDirectory(const std::string& dir_name = "logs");
    void writeLog(const std::string& message);

private:
    std::string config_path_;
};
