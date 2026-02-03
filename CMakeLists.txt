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

# nlohmann/json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# cpp-httplib
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.14.1
)
FetchContent_MakeAvailable(httplib)

# Add include directory
include_directories(include)

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
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    nlohmann_json::nlohmann_json 
    httplib::httplib
)

if(OPENSSL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()
