#include <iostream>
#include "SpaceApp.h"

/**
 * @file main.cpp
 * @brief Entry point for the Space Exploration Tracker.
 * 
 * Initializes the SpaceApp, which handles background data fetching
 * and the graphical user interface.
 */
int main() {
    try {
        // Initialize the main application controller
        SpaceApp app;

        // Start background threads and initialize UI systems
        app.start();

        // Run the main UI loop (blocks until window is closed)
        app.run();

        // Cleanup and shutdown
        app.stop();

    } catch (const std::exception& e) {
        std::cerr << "\n[CRITICAL ERROR] " << e.what() << "\n";
        // On Windows, the user might need a pause to see the error
        std::cout << "Press Enter to exit...";
        std::cin.ignore();
        return 1;
    }

    return 0;
}
