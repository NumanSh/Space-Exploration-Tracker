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
    
    // Internal UI logic
    void drawDashboard(const std::vector<Event>& events);
    void drawMenuBar();
    void drawEventTable(const std::vector<Event>& events);
};
