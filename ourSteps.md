# Project Development Steps - Space Exploration Tracker

## Phase 1: Planning and Setup (Completed)
- [x] Initialized Git repository with professional `.gitignore` and `README.md`.
- [x] Developed documentation for Git best practices and Conventional Commits.
- [x] Established project directory structure (`src`, `include`).
- [x] Configured `CMakeLists.txt` with `FetchContent` to manage dependencies (`nlohmann/json`, `cpp-httplib`).
- [x] Integrated OpenSSL for secure HTTPS communication with NASA APIs.
- [x] Implemented `DataFetcher` to successfully pull asteroid data from NASA NeoWs.
- [x] Verified connectivity with a successful console test.

## Phase 2: Core Functionality and Data Management (Completed)
- [x] Implemented `ConfigManager` for file I/O using `std::fstream` and `std::filesystem`.
- [x] Added `UserSettings` persistence (saving/loading from `settings.json`).
- [x] Implemented Background Threading using `std::thread` to poll APIs without blocking the UI.
- [x] Ensured Concurrency Safety using `std::mutex` and `std::atomic` for shared data access.
- [x] Implemented error logging system with timestamped logs in `/logs/`.
- [x] Verified background updates and persistent settings in an interactive console dashboard.

## Phase 3: UI Integration and Advanced Features (In Progress)
- [ ] Integrating **Dear ImGui** for the graphical user interface.
- [ ] Setting up GLFW and OpenGL backend for the dashboard.
- [ ] Developing the main Dashboard UI with tables and astronomical visualizations.
- [ ] Implementing the "What-if" Simulator for hypothetical asteroid impacts.
- [ ] Integrating **ImPlot** for orbital trajectory visualization.
