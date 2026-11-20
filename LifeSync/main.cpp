#include "include/Planner.h"
#include "include/FileManager.h"
#include "include/Menu.h"
#include "include/AppConfig.h"
#include <iostream>
#include <filesystem>

int main(int /*argc*/, char* argv[]) {
    // OOP Concept: shared_ptr — one AppConfig instance created here and shared
    // by both Planner and Menu.  Changing cfg->currencySymbol at runtime would
    // propagate to both objects without any copying (shared ownership).
    auto cfg = AppConfig::makeDefault();

    // OOP Concept: Composition - Planner owns all data; FileManager handles persistence
    // Locate data directory reliably:
    // Always prefer the main project workspace data folder if it exists
    std::filesystem::path dataDir = "D:/OOP PROJECT/LifeSync/data";
    if (!std::filesystem::exists(dataDir)) {
        if (std::filesystem::exists("data")) {
            dataDir = "data";
        } else {
            dataDir = std::filesystem::path(argv[0]).parent_path() / "data";
        }
    }
    std::filesystem::create_directories(dataDir);
    std::string taskFile    = (dataDir / "tasks.txt").string();
    std::string expenseFile = (dataDir / "expenses.txt").string();

    Planner     planner(cfg);           // shared_ptr passed to Planner
    FileManager fileManager(taskFile, expenseFile);

    // Load data at start-up (prints warning if file is missing - does NOT crash)
    fileManager.loadAll(planner);

    // OOP Concept: Association - Menu uses Planner and FileManager
    // planner and menu share the SAME cfg object (shared_ptr ref count = 2)
    Menu menu(planner, fileManager, cfg);
    menu.run();

    // Auto-save on exit is handled inside Menu::run()
    return 0;
}
