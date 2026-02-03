#pragma once

#include <string>
#include <vector>
#include "Event.h"

// Forward declarations for ImGui-related pointers
struct GLFWwindow;

/**
 * @class UIManager
 * @brief Manages the Dear ImGui lifecycle and rendering.
 * 
 * Responsible for creating the window, initializing the OpenGL backend,
 * and rendering the astronomical dashboard.
 */
class UIManager {
public:
    UIManager(const std::string& title = "Space Exploration Tracker");
    ~UIManager();

    /**
     * @brief Initializes GLFW, OpenGL, and ImGui.
     * @return true if initialization succeeded.
     */
    bool init();

    /**
     * @brief Checks if the window should close.
     */
    bool shouldClose() const;

    /**
     * @brief Starts a new frame, clears the screen, and renders the UI.
     * @param events The latest astronomical events to display.
     */
    void render(const std::vector<Event>& events);

    /**
     * @brief Closes the window and cleans up resources.
     */
    void cleanup();

private:
    std::string title_;
    GLFWwindow* window_ = nullptr;
    
    // Simulator State
    float sim_asteroid_mass = 1000.0f;
    float sim_impact_angle = 45.0f;
    float sim_predicted_crater = 0.0f;

    // Internal UI logic
    void drawDashboard(const std::vector<Event>& events);
    void drawMenuBar();
    void drawEventTable(const std::vector<Event>& events);
    void drawCharts(const std::vector<Event>& events);
    void drawSimulator();
    void drawSettingsPopup();
};
