#include <iostream>
#include <vector>
#include <thread>
#include "DataFetcher.h"
#include "Event.h"

void printEvents(const std::vector<Event>& events) {
    std::cout << "\n--- Near Earth Objects (Asteroids) ---\n";
    if (events.empty()) {
        std::cout << "No events found or fetch failed.\n";
        return;
    }

    for (const auto& e : events) {
        std::cout << "ID: " << e.id << " | Name: " << e.name 
                  << " | Date: " << e.date_str;
        if (e.is_hazardous) std::cout << " [HAZARDOUS]";
        std::cout << "\n";
        if (e.velocity_kph > 0) std::cout << "  - Velocity: " << e.velocity_kph << " km/h\n";
        if (e.close_approach_km > 0) std::cout << "  - Miss Dist: " << e.close_approach_km << " km\n";
        std::cout << "  - Link: " << e.details.at("nasa_jpl_url") << "\n";
    }
    std::cout << "--------------------------------------\n";
}

int main() {
    std::cout << "Space Exploration Tracker - Console Test\n";
    
    // IMPORTANT: In a real scenario, we would ask the user for their key.
    // For now, we will try with DEMO_KEY.
    DataFetcher fetcher("DEMO_KEY");

    // Get today's date for the query (Simplified hardcoded for test, ideally dynamic)
    // NASA API format: YYYY-MM-DD
    std::string start_date = "2024-01-01"; 
    std::string end_date = "2024-01-02";

    std::cout << "Fetching NeoWs data for " << start_date << " to " << end_date << "...\n";

    std::vector<Event> events;
    bool success = fetcher.fetchNeoWsFeed(start_date, end_date, events);

    if (success) {
        std::cout << "Successfully fetched " << events.size() << " events.\n";
        printEvents(events);
    } else {
        std::cerr << "Failed to fetch data. Check your internet or API key limits.\n";
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.ignore();
    return 0;
}
