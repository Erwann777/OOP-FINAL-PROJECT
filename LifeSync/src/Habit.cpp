#include "../include/Habit.h"
#include "../include/Exceptions.h"
#include "../include/Constants.h"
#include <iostream>
#include <sstream>

// OOP Concept: Inheritance - constructor delegates to base via initialiser list
Habit::Habit(int id, const std::string& title, const Date& dueDate,
             int currentStreak, int bestStreak, bool completed)
    : Task(id, title, dueDate, completed),
      currentStreak(currentStreak), bestStreak(bestStreak) {}

void Habit::setCurrentStreak(int s) {
    if (s < 0) throw InvalidInputException("Streak cannot be negative.");
    currentStreak = s;
}

void Habit::setBestStreak(int s) {
    if (s < 0) throw InvalidInputException("Best streak cannot be negative.");
    bestStreak = s;
}

void Habit::recordCompletion() {
    completed = true;
    currentStreak++;
    if (currentStreak > bestStreak) bestStreak = currentStreak;
}

// OOP Concept: Polymorphism - priority rises when streak is at risk
double Habit::getPriorityScore() const {
    if (completed) return 0.0;

    int days = dueDate.daysUntil();

    // Base urgency - if today is due date or overdue
    double urgency = 0.0;
    if (days <= 0)      urgency = 80.0;
    else if (days <= 1) urgency = 60.0;
    else                urgency = 30.0;

    // Streak risk bonus: the longer the streak, the more important not to break it
    // OOP Concept: constexpr — LifeSync::STREAK_MULTIPLIER and MAX_STREAK_BONUS
    double streakBonus = std::min(static_cast<double>(currentStreak) * LifeSync::STREAK_MULTIPLIER,
                                  LifeSync::MAX_STREAK_BONUS);

    return urgency + streakBonus;
}

void Habit::displaySummary() const {
    std::cout << "[Habit #" << id << "] " << title << "\n"
              << "  Due     : " << dueDate << "\n"
              << "  Streak  : " << currentStreak
              << " days (best: " << bestStreak << ")\n"
              << "  Status  : " << (completed ? "Done today" : "Pending") << "\n"
              << "  Priority: " << getPriorityScore() << "\n";
}

std::string Habit::serialise() const {
    // Format: Habit|id|title|DD/MM/YYYY|completed|currentStreak|bestStreak
    std::ostringstream oss;
    oss << "Habit|" << id << "|" << title << "|"
        << dueDate.toString() << "|" << completed << "|"
        << currentStreak << "|" << bestStreak;
    return oss.str();
}
