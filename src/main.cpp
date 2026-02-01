#include <iostream>
#include <nlohmann/json.hpp>
#include <httplib.h>

int main() {
    std::cout << "Space Exploration Tracker Initialized!" << std::endl;
    
    // Simple JSON usage check
    nlohmann::json test_json = { {"status", "ready"}, {"version", 1.0} };
    std::cout << "JSON Test: " << test_json.dump(4) << std::endl;

    // HTTP check (non-HTTPS first to test baseline)
    httplib::Client cli("http://httpbin.org");
    if (auto res = cli.Get("/get")) {
        if (res->status == 200) {
            std::cout << "HTTP Connection Test: Success!" << std::endl;
        } else {
            std::cout << "HTTP Connection Test: Error Status " << res->status << std::endl;
        }
    } else {
        auto err = res.error();
        std::cout << "HTTP Connection Test: Failed with error code " << (int)err << std::endl;
    }

    return 0;
}
