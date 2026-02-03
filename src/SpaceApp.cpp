#include "SpaceApp.h"
#include <iostream>
#include <chrono>

SpaceApp::SpaceApp() : fetcher_("DEMO_KEY"), config_("settings.json") {
    if (!config_.loadSettings(settings_)) {
        std::cout << "No settings found, using defaults.\n";
        settings_.user_name = "Space Explorer";
        config_.saveSettings(settings_);
    }
}

SpaceApp::~SpaceApp() {
    stop();
}

void SpaceApp::start() {
    if (running_) return;

    if (!ui_.init()) {
        config_.writeLog("Failed to initialize UI. Exiting.");
        return;
    }

    // Do an initial fetch immediately so data is ready when UI appears
    std::cout << "[Startup] Performing initial data fetch...\n";
    std::vector<Event> initial_events;
    if (fetcher_.fetchNeoWsFeed("2024-01-01", "2024-01-02", initial_events)) {
        std::lock_guard<std::mutex> lock(data_mutex_);
        current_events_ = initial_events;
        std::cout << "[Startup] Loaded " << current_events_.size() << " events.\n";
        config_.writeLog("Initial fetch: " + std::to_string(current_events_.size()) + " asteroids.");
    }

    running_ = true;
    poller_thread_ = std::thread(&SpaceApp::pollingLoop, this);
    config_.writeLog("Application started background poller.");
}

void SpaceApp::run() {
    while (!ui_.shouldClose()) {
        std::vector<Event> events = getLatestEvents();
        ui_.render(events);
    }
}

void SpaceApp::stop() {
    if (!running_) return;

    running_ = false;
    if (poller_thread_.joinable()) {
        poller_thread_.join();
    }
    config_.writeLog("Application stopped background poller.");
}

std::vector<Event> SpaceApp::getLatestEvents() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    return current_events_;
}

UserSettings SpaceApp::getUserSettings() {
    // Basic settings are small, mutex not strictly needed for trivial fields 
    // but good practice if more complex.
    return settings_;
}

void SpaceApp::setUserSettings(const UserSettings& settings) {
    settings_ = settings;
    config_.saveSettings(settings_);
}

void SpaceApp::pollingLoop() {
    while (running_) {
        std::cout << "[Background] Fetching updates...\n";
        
        std::vector<Event> new_events;
        // Fetch for current date (hardcoded for now as demo)
        if (fetcher_.fetchNeoWsFeed("2024-01-01", "2024-01-01", new_events)) {
            std::lock_guard<std::mutex> lock(data_mutex_);
            current_events_ = new_events;
            std::cout << "[Background] Successfully updated " << current_events_.size() << " events.\n";
            config_.writeLog("Fetched " + std::to_string(current_events_.size()) + " asteroids.");
        }

        // Wait for 30 seconds or until stopped
        for (int i = 0; i < 30 && running_; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
