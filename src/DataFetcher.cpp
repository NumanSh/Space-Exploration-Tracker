#include "DataFetcher.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

// Helper to safely convert string to double without crashing
static double stod_safe(const std::string& str) {
    if (str.empty()) return 0.0;
    try {
        return std::stod(str);
    } catch (...) {
        return 0.0;
    }
}

DataFetcher::DataFetcher(const std::string& api_key) : api_key_(api_key) {}

std::string DataFetcher::fetchRaw(const std::string& host, const std::string& path) {
#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
    if (host.find("https") != std::string::npos) {
        throw std::runtime_error("HTTPS support is not enabled in this build. Please ensure OpenSSL is found by CMake.");
    }
#endif
    httplib::Client cli(host);
    
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    cli.enable_server_certificate_verification(false);
#endif
    
    cli.set_connection_timeout(10); 
    cli.set_read_timeout(10);

    auto res = cli.Get(path);
    if (res) {
        if (res->status == 200) {
            return res->body;
        } else {
            std::cerr << "HTTP Error: " << res->status << " for " << path << std::endl;
        }
    } else {
        auto err = res.error();
        std::cerr << "HTTP Connection Failed: " << (int)err << std::endl;
        
#ifndef CPPHTTPLIB_OPENSSL_SUPPORT
        if (host.find("https") != std::string::npos) {
            std::cerr << "TIP: OpenSSL support is NOT enabled. HTTPS requests (NASA) will fail.\n";
        }
#endif
    }
    return "";
}

bool DataFetcher::fetchNeoWsFeed(const std::string& start_date, const std::string& end_date, std::vector<Event>& out_events) {
    // NASA usually requires HTTPS
    std::string host = "https://api.nasa.gov";
    std::string path = "/neo/rest/v1/feed?start_date=" + start_date + 
                       "&end_date=" + end_date + 
                       "&api_key=" + api_key_;

    std::string response = fetchRaw(host, path);
    if (response.empty()) return false;

    try {
        auto j = json::parse(response);
        
        if (j.contains("near_earth_objects")) {
            for (auto& element : j["near_earth_objects"].items()) {
                std::string date = element.key();
                auto asteroids = element.value();

                for (const auto& asteroid : asteroids) {
                    Event e;
                    e.id = asteroid.value("id", "0");
                    e.name = asteroid.value("name", "Unknown");
                    e.type = "Asteroid";
                    e.date_str = date;
                    e.is_hazardous = asteroid.value("is_potentially_hazardous_asteroid", false);
                    e.magnitude = asteroid.value("absolute_magnitude_h", 0.0);
                    
                    // Safe access to details
                    std::string jpl_url = asteroid.value("nasa_jpl_url", "");
                    e.details["nasa_jpl_url"] = jpl_url.empty() ? "N/A" : jpl_url;

                    if (asteroid.contains("close_approach_data") && !asteroid["close_approach_data"].empty()) {
                        auto approach = asteroid["close_approach_data"][0];
                        
                        if (approach.contains("relative_velocity")) {
                            e.velocity_kph = stod_safe(approach["relative_velocity"].value("kilometers_per_hour", "0"));
                        }

                        if (approach.contains("miss_distance")) {
                            e.close_approach_km = stod_safe(approach["miss_distance"].value("kilometers", "0"));
                        }
                    }
                    out_events.push_back(e);
                }
            }
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Parsing Error: " << e.what() << std::endl;
    }

    return false;
}
