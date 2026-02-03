#include "SpaceApp.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

// Helper to get current date as YYYY-MM-DD
static std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

SpaceApp::SpaceApp() : fetcher_("A1dydBjQ4zhDjl5jIqTqw2SNbSdvQECHmdJ6aeBy"), config_("settings.json") {
    if (!config_.loadSettings(settings_)) {
        std::cout << "No settings found, using defaults.\n";
        settings_.user_name = "Space Explorer";
        settings_.refresh_interval = 60; // 1 minute
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

    // Do an initial fetch immediately
    std::string today = getCurrentDate();
    std::cout << "[Startup] Fetching data for " << today << "...\n";
    std::vector<Event> initial_events;
    if (fetcher_.fetchNeoWsFeed(today, today, initial_events)) {
        std::lock_guard<std::mutex> lock(data_mutex_);
        current_events_ = initial_events;
        std::cout << "[Startup] Loaded " << current_events_.size() << " events.\n";
        config_.writeLog("Initial fetch: " + std::to_string(current_events_.size()) + " asteroids for " + today);
    }

    running_ = true;
    poller_thread_ = std::thread(&SpaceApp::pollingLoop, this);
    config_.writeLog("Application started background poller with interval: " + std::to_string(settings_.refresh_interval) + "s");
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
    return settings_;
}

void SpaceApp::setUserSettings(const UserSettings& settings) {
    settings_ = settings;
    config_.saveSettings(settings_);
}

void SpaceApp::pollingLoop() {
    while (running_) {
        std::string today = getCurrentDate();
        std::cout << "[Background] Fetching updates for " << today << "...\n";
        
        std::vector<Event> new_events;
        if (fetcher_.fetchNeoWsFeed(today, today, new_events)) {
            std::lock_guard<std::mutex> lock(data_mutex_);
            current_events_ = new_events;
            std::cout << "[Background] Updated " << current_events_.size() << " events.\n";
            config_.writeLog("Background update successful.");
        }

        // Wait for interval from settings (default 60s)
        for (int i = 0; i < settings_.refresh_interval && running_; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
