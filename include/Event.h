#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

/**
 * @struct Event
 * @brief Represents a single astronomical event or near-earth object.
 * 
 * This structure holds core data for various space events, primarily used
 * for asteroid tracking (NASA NeoWs) but extensible to other types like satellite passes.
 */
struct Event {
    std::string id;             ///< Unique identifier from the source API
    std::string name;           ///< Common name of the object
    std::string type;           ///< Category of the event (e.g., "Asteroid", "Launch")
    std::string date_str;       ///< ISO 8601 formatted date for simple storage and display
    
    // Core physical data
    double magnitude = 0.0;       ///< Absolute magnitude (size/brightness)
    bool is_hazardous = false;    ///< Potentially hazardous flag
    double close_approach_km = 0.0; ///< Miss distance from Earth in kilometers
    double velocity_kph = 0.0;     ///< Relative velocity in kilometers per hour
    
    /**
     * @brief A flexible map for metadata that doesn't fit the core structure.
     * Examples: NASA JPL URL, orbital class, spectral type.
     */
    std::unordered_map<std::string, std::string> details;

    /**
     * @brief Helper function for console debugging and logging.
     * @return A formatted string summarizing the event.
     */
    std::string toString() const {
        return "[" + type + "] " + name + " (ID: " + id + ") - " + date_str;
    }
};
