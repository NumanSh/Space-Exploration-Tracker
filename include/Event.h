#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

struct Event {
    std::string id;
    std::string name;
    std::string type; // e.g., "Asteroid", "Launch"
    std::string date_str; // ISO 8601 string for simplicity in display
    
    // Core data
    double magnitude = 0.0;
    bool is_hazardous = false;
    double close_approach_km = 0.0;
    double velocity_kph = 0.0;
    
    // Flexible details for specific types (Launch provider, orbital class, etc.)
    std::unordered_map<std::string, std::string> details;

    // Helper for debugging
    std::string toString() const {
        return "[" + type + "] " + name + " (ID: " + id + ") - " + date_str;
    }
};
