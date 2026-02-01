#include "DataFetcher.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

DataFetcher::DataFetcher(const std::string& api_key) : api_key_(api_key) {}

std::string DataFetcher::fetchRaw(const std::string& host, const std::string& path) {
    // Determine if we need HTTPS based on host (NASA is HTTPS)
    // For simplicity in this phase, we assume HTTPS for NASA types
    
    httplib::Client cli(host);
    cli.enable_server_certificate_verification(false); // Simplified for dev, enable strictly in prod
    
    // Set a reasonable timeout
    cli.set_connection_timeout(10); 
    cli.set_read_timeout(10);

    auto res = cli.Get(path);
    if (res) {
        if (res->status == 200) {
            return res->body;
        } else {
            std::cerr << "HTTP Error: " << res->status << " for " << host << path << std::endl;
        }
    } else {
        auto err = res.error();
        std::cerr << "HTTP Connection Failed: " << (int)err << std::endl;
    }
    return "";
}

bool DataFetcher::fetchNeoWsFeed(const std::string& start_date, const std::string& end_date, std::vector<Event>& out_events) {
    std::string host = "https://api.nasa.gov";
    std::string path = "/neo/rest/v1/feed?start_date=" + start_date + 
                       "&end_date=" + end_date + 
                       "&api_key=" + api_key_;

    std::string response = fetchRaw(host, path);
    if (response.empty()) return false;

    try {
        auto j = json::parse(response);
        
        // Parse "near_earth_objects"
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
                    e.magnitude = asteroid["absolute_magnitude_h"].get<double>();
                    e.details["nasa_jpl_url"] = asteroid.value("nasa_jpl_url", "");

                    // Get close approach data if available
                    if (asteroid.contains("close_approach_data") && !asteroid["close_approach_data"].empty()) {
                        auto approach = asteroid["close_approach_data"][0];
                        
                        // Parse velocity
                        if (approach["relative_velocity"].contains("kilometers_per_hour")) {
                            std::string vel_str = approach["relative_velocity"]["kilometers_per_hour"];
                            e.velocity_kph = std::stod(vel_str);
                        }

                        // Parse miss distance
                        if (approach["miss_distance"].contains("kilometers")) {
                            std::string dist_str = approach["miss_distance"]["kilometers"];
                            e.close_approach_km = std::stod(dist_str);
                        }
                    }

                    out_events.push_back(e);
                }
            }
            return true;
        }
    } catch (const json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing asteroid data: " << e.what() << std::endl;
    }

    return false;
}
