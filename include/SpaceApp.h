#pragma once

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include "Event.h"
#include "DataFetcher.h"
#include "ConfigManager.h"

class SpaceApp {
public:
    SpaceApp();
    ~SpaceApp();

    void start();
    void stop();

    // Data access
    std::vector<Event> getLatestEvents();
    UserSettings getUserSettings();
    void setUserSettings(const UserSettings& settings);

private:
    void pollingLoop();

    DataFetcher fetcher_;
    ConfigManager config_;
    UserSettings settings_;

    std::vector<Event> current_events_;
    std::mutex data_mutex_;
    
    std::atomic<bool> running_{false};
    std::thread poller_thread_;
};
