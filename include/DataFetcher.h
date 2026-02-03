#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Event.h"

// Callback type for async operations (future proofing)
using DataCallback = std::function<void(bool success, const std::string& message)>;

/**
 * @class DataFetcher
 * @brief Handles all HTTP/HTTPS communication with external space APIs.
 * 
 * Uses cpp-httplib for networking and nlohmann/json for data parsing.
 * Requires OpenSSL for HTTPS support (mandatory for NASA APIs).
 */
class DataFetcher {
public:
    /**
     * @brief Construct a new Data Fetcher object
     * @param api_key The NASA API key (defaults to "DEMO_KEY")
     */
    DataFetcher(const std::string& api_key = "DEMO_KEY");
    
    /**
     * @brief Fetches Near Earth Object data from NASA's NeoWs API.
     * @param start_date Beginning of date range (YYYY-MM-DD)
     * @param end_date End of date range (YYYY-MM-DD)
     * @param out_events Vector to be populated with successfully parsed events.
     * @return true if fetch and parse succeeded, false otherwise.
     */
    bool fetchNeoWsFeed(const std::string& start_date, const std::string& end_date, std::vector<Event>& out_events);

    /**
     * @brief Performs a raw HTTP/S GET request.
     * @param host The hostname (e.g., https://api.nasa.gov)
     * @param path The endpoint path with query parameters.
     * @return std::string The response body or empty string on failure.
     */
    std::string fetchRaw(const std::string& host, const std::string& path);

private:
    std::string api_key_;
};
