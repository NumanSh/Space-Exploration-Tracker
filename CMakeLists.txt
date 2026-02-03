cmake_minimum_required(VERSION 3.14)
project(SpaceTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fix for older subprojects
if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

# Include FetchContent
include(FetchContent)

# 1. nlohmann/json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# 2. cpp-httplib
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.14.1
)
FetchContent_MakeAvailable(httplib)

# 3. GLFW (Windowing & Inputs)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# 4. Dear ImGui (UI Framework)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.89.9
)
FetchContent_MakeAvailable(imgui)

# 5. ImPlot (Data Visualization)
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG v0.16
)
FetchContent_MakeAvailable(implot)

# Add include directory
include_directories(include)
include_directories(${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/backends)
include_directories(${implot_SOURCE_DIR})

# --- OPENSSL SETUP ---
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_USE_STATIC_LIBS FALSE)
find_package(OpenSSL)

if(OPENSSL_FOUND)
    message(STATUS "!!! OPENSSL FOUND: ${OPENSSL_VERSION} !!!")
    add_compile_definitions(CPPHTTPLIB_OPENSSL_SUPPORT)
    include_directories(${OPENSSL_INCLUDE_DIR})
else()
    message(WARNING "!!! OPENSSL NOT FOUND !!! HTTPS will fail.")
endif()

# Define source files
set(SOURCES
    src/main.cpp
    src/DataFetcher.cpp
    src/ConfigManager.cpp
    src/SpaceApp.cpp
    src/UIManager.cpp
    # ImGui Implementation Files (will be added as we implement the UI)
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    nlohmann_json::nlohmann_json 
    httplib::httplib
    glfw
    opengl32
)

if(OPENSSL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()
