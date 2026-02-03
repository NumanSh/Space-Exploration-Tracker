#include "UIManager.h"
#include <imgui.h>
#include <implot.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

UIManager::UIManager(const std::string& title) : title_(title) {}

UIManager::~UIManager() {
    cleanup();
}

bool UIManager::init() {
    // 1. Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    // OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. Create Window
    window_ = glfwCreateWindow(1280, 800, title_.c_str(), NULL, NULL);
    if (!window_) {
        std::cerr << "Failed to create GLFW window\n";
        return false;
    }
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable vsync

    // 3. Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext(); // Initialize ImPlot
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style (Go with Dark mode as per request for "Rich Aesthetics")
    ImGui::StyleColorsDark();

    // 4. Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

bool UIManager::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

void UIManager::render(const std::vector<Event>& events) {
    // Poll and handle events (inputs, window resize, etc.)
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw our custom content
    drawDashboard(events);

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window_, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f); // Sleek dark slate background
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);
}

void UIManager::drawDashboard(const std::vector<Event>& events) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("DashboardSpace", nullptr, 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

    drawMenuBar();

    ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "SPACE EXPLORATION DASHBOARD");
    ImGui::SameLine(ImGui::GetWindowWidth() - 180);
    
    if (events.empty()) {
        ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "● Status: CONNECTING");
    } else {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "● Status: NASA ONLINE");
    }
    ImGui::Separator();

    if (ImGui::BeginTabBar("MainTabs")) {
        if (ImGui::BeginTabItem("Astronomical Events")) {
            if (events.empty()) {
                ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth()/2 - 100, ImGui::GetWindowHeight()/2));
                ImGui::Text("Requesting Data from NASA NeoWs...");
            } else {
                if (ImGui::BeginChild("EventList", ImVec2(0, 0), true)) {
                    drawEventTable(events);
                }
                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Data Analysis")) {
            drawCharts(events);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("What-If Simulator")) {
            drawSimulator();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void UIManager::drawCharts(const std::vector<Event>& events) {
    if (events.empty()) {
        ImGui::Text("Waiting for NASA telemetry data...");
        return;
    }

    static std::vector<float> velocities;
    static std::vector<float> x_indices;
    velocities.clear();
    x_indices.clear();

    for (size_t i = 0; i < events.size(); ++i) {
        velocities.push_back((float)events[i].velocity_kph);
        x_indices.push_back((float)i);
    }

    ImGui::BulletText("Velocity Distribution (km/h)");
    if (ImPlot::BeginPlot("Object Velocities", ImVec2(-1, 350))) {
        ImPlot::SetupAxes("Object Rank", "Velocity (km/h)");
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 150000);
        ImPlot::PlotBars("Velocity", x_indices.data(), velocities.data(), (int)velocities.size(), 0.5);
        ImPlot::EndPlot();
    }

    ImGui::Separator();
    ImGui::TextWrapped("This chart visualizes the relative speeds of currently tracked Near-Earth Objects. Higher bars indicate faster-moving celestial bodies.");
}

void UIManager::drawSimulator() {
    ImGui::Columns(2, "SimColumns", true);
    
    ImGui::TextColored(ImVec4(1, 0.8f, 0, 1), "HYPOTHETICAL IMPACT CONFIGURATOR");
    ImGui::Separator();
    
    ImGui::Spacing();
    ImGui::Text("Step 1: Choose a Preset (optional)");
    if (ImGui::Button("Small Meteor (Chelyabinsk)")) {
        sim_asteroid_mass = 12000.0f;
        sim_impact_angle = 19.0f;
    }
    ImGui::SameLine();
    if (ImGui::Button("Tunguska Event")) {
        sim_asteroid_mass = 100000.0f;
        sim_impact_angle = 20.0f;
    }
    if (ImGui::Button("Dinosaur Extinction (Chicxulub)")) {
        sim_asteroid_mass = 1.0e12f; // Trillions of tons
        sim_impact_angle = 20.0f;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Step 2: Custom Parameters");
    
    ImGui::SliderFloat("Mass (Tons)", &sim_asteroid_mass, 1.0f, 1000000.0f, "%.0f");
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("The weight of the object affects the total kinetic energy release.");
    
    ImGui::SliderFloat("Velocity (km/s)", &sim_impact_angle, 1.0f, 72.0f, "%.1f");
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Space velocity is the most critical factor in impact force.");
    
    ImGui::Spacing();
    if (ImGui::Button("CALCULATE IMPACT FORCE", ImVec2(-1, 40))) {
        // Simple Physics: E = 0.5 * m * v^2
        double velocity_ms = (double)sim_impact_angle * 1000.0;
        double mass_kg = (double)sim_asteroid_mass * 1000.0;
        double energy = 0.5 * mass_kg * (velocity_ms * velocity_ms);
        
        // Crater estimate (Simplified)
        sim_predicted_crater = (float)(std::pow(energy, 1.0/3.4) * 0.05);
    }

    ImGui::NextColumn();

    ImGui::TextColored(ImVec4(0, 1, 0, 1), "SCIENTIFIC PROJECTION");
    ImGui::BeginChild("ResultsFrame", ImVec2(0, 0), true);
    
    ImU32 severity_color = IM_COL32(200, 200, 200, 255);
    std::string severity_text = "Minor Impact";

    if (sim_predicted_crater > 5000) {
        severity_color = IM_COL32(255, 0, 0, 255);
        severity_text = "GLOBAL CATASTROPHE";
    } else if (sim_predicted_crater > 1000) {
        severity_color = IM_COL32(255, 128, 0, 255);
        severity_text = "REGIONAL DESTRUCTION";
    } else if (sim_predicted_crater > 100) {
        severity_color = IM_COL32(255, 255, 0, 255);
        severity_text = "LOCAL IMPACT EVENT";
    }

    ImGui::Text("Event Type:");
    ImGui::PushStyleColor(ImGuiCol_Text, severity_color);
    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("%s", severity_text.c_str());
    ImGui::PopStyleColor();
    ImGui::SetWindowFontScale(1.0f);
    
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::Text("Estimated Crater Diameter:");
    ImGui::TextColored(ImVec4(1, 0.5f, 0.2f, 1), "%.2f Meters", sim_predicted_crater);
    
    ImGui::Spacing();
    ImGui::Text("Impact Energy:");
    ImGui::Text("%.2e Joules", 0.5 * (sim_asteroid_mass * 1000.0) * std::pow(sim_impact_angle * 1000.0, 2));

    ImGui::EndChild();

    ImGui::Columns(1);
}

void UIManager::drawMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", "Alt+F4")) { glfwSetWindowShouldClose(window_, true); }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void UIManager::drawEventTable(const std::vector<Event>& events) {
    static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("Events", 6, flags)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Date");
        ImGui::TableSetupColumn("Velocity (km/h)");
        ImGui::TableSetupColumn("Hazardous");
        ImGui::TableSetupColumn("Details");
        ImGui::TableHeadersRow();

        for (const auto& e : events) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", e.id.c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", e.name.c_str());
            ImGui::TableSetColumnIndex(2); ImGui::Text("%s", e.date_str.c_str());
            ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f", e.velocity_kph);
            ImGui::TableSetColumnIndex(4); 
            if (e.is_hazardous) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "YES");
            } else {
                ImGui::Text("No");
            }
            ImGui::TableSetColumnIndex(5);
            if (ImGui::SmallButton((std::string("View##") + e.id).c_str())) {
                ImGui::OpenPopup((std::string("AsteroidDetail") + e.id).c_str());
            }

            // Details Popup
            if (ImGui::BeginPopupModal((std::string("AsteroidDetail") + e.id).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Full Object Details: %s", e.name.c_str());
                ImGui::Separator();
                ImGui::Text("ID: %s", e.id.c_str());
                ImGui::Text("Absolute Magnitude: %.2f", e.magnitude);
                ImGui::Text("Hazard Rating: %s", e.is_hazardous ? "Potentially Hazardous" : "Safe");
                ImGui::Text("Miss Distance: %.2f km", e.close_approach_km);
                
                auto it = e.details.find("nasa_jpl_url");
                if (it != e.details.end()) {
                    ImGui::Text("Database URL: %s", it->second.c_str());
                }

                if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
        }
        ImGui::EndTable();
    }
}

void UIManager::drawSettingsPopup() {}

void UIManager::cleanup() {
    if (window_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        glfwDestroyWindow(window_);
        glfwTerminate();
        window_ = nullptr;
    }
}
