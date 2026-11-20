#ifndef HABIT_H
#define HABIT_H

#include "Task.h"
#include <string>

// OOP Concept: Inheritance (IS-A) - Habit IS-A Task
// Recurring daily habit; priority rises when streak is at risk
class Habit : public Task {
private:
    int currentStreak; // consecutive days completed
    int bestStreak;    // all-time best streak

public:
    // Constructor calls base constructor via initialiser list
    Habit(int id, const std::string& title, const Date& dueDate,
          int currentStreak = 0, int bestStreak = 0, bool completed = false);

    // Getters (Encapsulation)
    int getCurrentStreak() const { return currentStreak; }
    int getBestStreak()    const { return bestStreak; }

    // Setters (Encapsulation)
    void setCurrentStreak(int s);
    void setBestStreak(int s);

    // Mark the habit done today - updates streak
    void recordCompletion();

    // OOP Concept: Polymorphism - override pure virtuals
    double      getPriorityScore() const override;
    std::string getTypeName()      const override { return "Habit"; }
    void        displaySummary()   const override;

    // Serialise to pipe-delimited string for file storage
    std::string serialise() const;
};

#endif // HABIT_H
