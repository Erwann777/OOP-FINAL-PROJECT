#include "../include/Assignment.h"
#include "../include/Exceptions.h"
#include "../include/Constants.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// OOP Concept: Inheritance - constructor delegates to base via initialiser list
Assignment::Assignment(int id, const std::string& title, const Date& dueDate,
                       const std::string& subject, double weight, bool completed)
    : Task(id, title, dueDate, completed), subject(subject), weight(weight) {}

void Assignment::setWeight(double w) {
    // OOP Concept: constexpr from LifeSync namespace — no magic numbers
    if (w < LifeSync::MIN_WEIGHT || w > LifeSync::MAX_WEIGHT)
        throw InvalidInputException("Weight must be between 0 and 100.");
    weight = w;
}

// OOP Concept: Polymorphism - each subclass calculates its own priority
double Assignment::getPriorityScore() const {
    if (completed) return 0.0;

    int days = dueDate.daysUntil(); // negative if overdue

    // Urgency: higher score when fewer days remain or already overdue
    double urgency = 0.0;
    if (days <= 0)       urgency = 100.0;          // overdue
    else if (days <= 1)  urgency = 90.0;
    else if (days <= 3)  urgency = 70.0;
    else if (days <= 7)  urgency = 50.0;
    else if (days <= 14) urgency = 30.0;
    else                 urgency = 10.0;

    // Weight factor (0-100 scale)
    double weightFactor = weight;

    return urgency * 0.6 + weightFactor * 0.4;
}

void Assignment::displaySummary() const {
    // OOP Concept: Polymorphism - this version runs when pointer is Assignment
    std::cout << "[Assignment #" << id << "] " << title << "\n"
              << "  Subject : " << subject << " | Weight: " << weight << "%\n"
              << "  Due     : " << dueDate << " | Status: "
              << (completed ? "Done" : "Pending") << "\n"
              << "  Priority: " << getPriorityScore() << "\n";
}

std::string Assignment::serialise() const {
    // Format: Assignment|id|title|DD/MM/YYYY|completed|subject|weight
    std::ostringstream oss;
    oss << "Assignment|" << id << "|" << title << "|"
        << dueDate.toString() << "|" << completed << "|"
        << subject << "|" << weight;
    return oss.str();
}
