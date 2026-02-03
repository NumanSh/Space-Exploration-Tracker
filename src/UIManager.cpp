#include "UIManager.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    window_ = glfwCreateWindow(1280, 720, title_.c_str(), NULL, NULL);
    if (!window_) {
        std::cerr << "Failed to create GLFW window\n";
        return false;
    }
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable vsync

    // 3. Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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
    // Create a fullscreen docking-like space
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("DashboardSpace", nullptr, 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

    drawMenuBar();

    ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "SPACE EXPLORATION DASHBOARD");
    ImGui::Separator();

    if (ImGui::BeginChild("EventList", ImVec2(0, 0), true)) {
        drawEventTable(events);
    }
    ImGui::EndChild();

    ImGui::End();
}

void UIManager::drawMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", "Alt+F4")) { glfwSetWindowShouldClose(window_, true); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            ImGui::MenuItem("User Profile");
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void UIManager::drawEventTable(const std::vector<Event>& events) {
    static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("Events", 5, flags)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Date");
        ImGui::TableSetupColumn("Velocity (km/h)");
        ImGui::TableSetupColumn("Hazardous");
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
        }
        ImGui::EndTable();
    }
}

void UIManager::cleanup() {
    if (window_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window_);
        glfwTerminate();
        window_ = nullptr;
    }
}
