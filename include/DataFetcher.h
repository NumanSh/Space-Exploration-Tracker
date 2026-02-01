#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Event.h"

// Callback type for async operations (future proofing)
using DataCallback = std::function<void(bool success, const std::string& message)>;

class DataFetcher {
public:
    DataFetcher(const std::string& api_key = "DEMO_KEY");
    
    // NASA Asteroids - NeoWs
    bool fetchNeoWsFeed(const std::string& start_date, const std::string& end_date, std::vector<Event>& out_events);

    // Generic fetch helper
    std::string fetchRaw(const std::string& host, const std::string& path);

private:
    std::string api_key_;
};
