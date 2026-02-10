
# Space Exploration and Astronomy Event Tracker
Developed by: Numan Sharabati & Hamza Salhab

An advanced C++ 20 application designed for real-time tracking of Near-Earth Objects (NEOs), orbital analysis, and interactive astronomical data visualization.

## Advanced C++ Features (Course Topics)
This project serves as a comprehensive implementation of advanced C++ concepts:

Multithreading & Concurrency:

Dedicated Background Polling Thread for non-blocking HTTP data retrieval.

Use of std::mutex and std::lock_guard to ensure Thread Safety when accessing shared mission data.

Implementation of std::atomic<bool> for efficient, lock-free application lifecycle management.

Modern STL & Data Structures:

Utilization of std::unordered_map for O(1) data lookups and std::vector for dynamic event storage.

Usage of std::filesystem for robust log management and persistent settings storage.

Networking & JSON Serialization:

Integration of httplib for asynchronous communication with NASA APIs.

Advanced JSON parsing and mapping using the nlohmann/json library.

## Core Features
NASA Real-time Telemetry: Fetches live asteroid (NeoWs) data, including velocity, miss distance, and hazardous classification.

Interactive Dashboard: Sleek, high-performance UI built with Dear ImGui featuring real-time data filtering.

Scientific Visualization: Integrated ImPlot charts for correlation analysis (e.g., Velocity vs. Distance).

Impact Simulator: A "What-if" physics engine to calculate kinetic energy release and crater diameter for hypothetical asteroid impacts.

## Tech Stack
Language: C++ 20 (Standard Core)

Build System: CMake (Minimum version 3.15)

Graphics: OpenGL 3.3 / GLFW

Libraries:

httplib: High-level HTTP client functionality.

nlohmann/json: Modern JSON for C++.

Dear ImGui & ImPlot: Immediate mode GUI and plotting.

## Getting Started
### Prerequisites
OpenSSL: Required for secure HTTPS communication with NASA servers.

C++20 Compiler: GCC 10+, Clang 10+, or MSVC 2019+.

### Build Instructions
