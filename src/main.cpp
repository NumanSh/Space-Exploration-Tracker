#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "SpaceApp.h"
#include "Event.h"

void displayDashboard(SpaceApp& app) {
    auto settings = app.getUserSettings();
    auto events = app.getLatestEvents();

    std::cout << "\n======================================\n";
    std::cout << " WELCOME, " << settings.user_name << "\n";
    std::cout << " Location: " << settings.location_name << "\n";
    std::cout << "======================================\n";
    
    if (events.empty()) {
        std::cout << "Waiting for initial data fetch...\n";
    } else {
        std::cout << "Latest Asteroid Briefing (" << events.size() << " objects):\n";
        for (size_t i = 0; i < std::min(events.size(), (size_t)5); ++i) {
            std::cout << " - " << events[i].name;
            if (events[i].is_hazardous) std::cout << " [!]";
            std::cout << " (Velocity: " << (int)events[i].velocity_kph << " km/h)\n";
        }
        if (events.size() > 5) std::cout << " ... and " << (events.size() - 5) << " more.\n";
    }
    std::cout << "======================================\n";
    std::cout << "Commands: [r] Refresh Display, [s] Change Name, [q] Quit\n";
    std::cout << "Choice: ";
}

int main() {
    try {
        SpaceApp app;
        app.start();

        char choice = ' ';
        while (choice != 'q') {
            displayDashboard(app);
            std::cin >> choice;

            if (choice == 's') {
                std::cout << "Enter new name: ";
                std::string res;
                std::cin >> res;
                
                auto settings = app.getUserSettings();
                settings.user_name = res;
                app.setUserSettings(settings);
                std::cout << "Settings saved to disk!\n";
            }
        }

        std::cout << "Shutting down...\n";
        app.stop();

    } catch (const std::exception& e) {
        std::cerr << "\n[CRITICAL ERROR] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
