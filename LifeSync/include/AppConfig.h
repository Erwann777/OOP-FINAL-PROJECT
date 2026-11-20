#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <string>
#include <memory>

// OOP Concept: shared_ptr / Shared Ownership
// AppConfig is a configuration record shared between Planner and Menu via
// std::shared_ptr.  A single instance is created in main() and passed to both
// objects; any runtime change to a field (e.g. currencySymbol) is immediately
// visible to both without copying.
struct AppConfig {
    std::string appName        = "LifeSync";
    std::string currencySymbol = "RM";      // used in all money output
    std::string dateFormat     = "DD/MM/YYYY";

    // Factory helper: returns a heap-allocated default config via shared_ptr
    static std::shared_ptr<AppConfig> makeDefault() {
        return std::make_shared<AppConfig>();
    }
};

#endif // APPCONFIG_H

