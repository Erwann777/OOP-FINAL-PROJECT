#include "../include/Appointment.h"
#include <iostream>
#include <sstream>

// OOP Concept: Inheritance - constructor delegates to base via initialiser list
Appointment::Appointment(int id, const std::string& title, const Date& dueDate,
                         const std::string& time, const std::string& location,
                         bool completed)
    : Task(id, title, dueDate, completed), time(time), location(location) {}

// OOP Concept: Polymorphism - priority is highest on the day of the event
double Appointment::getPriorityScore() const {
    if (completed) return 0.0;

    int days = dueDate.daysUntil();

    if (days == 0)       return 100.0;  // today: maximum priority
    else if (days < 0)   return 0.0;    // past events: lowest priority
    else if (days <= 1)  return 80.0;
    else if (days <= 3)  return 60.0;
    else if (days <= 7)  return 40.0;
    else                 return 20.0;
}

void Appointment::displaySummary() const {
    std::cout << "[Appointment #" << id << "] " << title << "\n"
              << "  Date    : " << dueDate << " at " << time << "\n"
              << "  Location: " << location << "\n"
              << "  Status  : " << (completed ? "Done" : "Pending") << "\n"
              << "  Priority: " << getPriorityScore() << "\n";
}

std::string Appointment::serialise() const {
    // Format: Appointment|id|title|DD/MM/YYYY|completed|time|location
    std::ostringstream oss;
    oss << "Appointment|" << id << "|" << title << "|"
        << dueDate.toString() << "|" << completed << "|"
        << time << "|" << location;
    return oss.str();
}
