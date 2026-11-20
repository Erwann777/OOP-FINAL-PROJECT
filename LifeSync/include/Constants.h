#ifndef CONSTANTS_H
#define CONSTANTS_H

// OOP Concept: Namespaces + constexpr
// Compile-time named constants shared across the entire LifeSync application.
// Grouping them in a namespace avoids global-scope name pollution.
namespace LifeSync {

    // Priority thresholds (days)
    constexpr int    URGENT_DAYS        = 3;    // <= 3 days → urgent
    constexpr int    NEAR_DAYS          = 7;    // <= 7 days → near

    // Assignment weight (percentage)
    constexpr double MIN_WEIGHT         = 0.0;
    constexpr double MAX_WEIGHT         = 100.0;

    // Bill priority tuning
    constexpr double MAX_AMOUNT_FACTOR  = 30.0; // cap on amount's priority contribution

    // Habit priority tuning
    constexpr double STREAK_MULTIPLIER  = 2.0;  // priority points per streak day
    constexpr double MAX_STREAK_BONUS   = 20.0; // cap on streak's priority contribution

} // namespace LifeSync

#endif // CONSTANTS_H

