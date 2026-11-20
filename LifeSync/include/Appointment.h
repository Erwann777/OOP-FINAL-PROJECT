#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "Task.h"
#include <string>

// OOP Concept: Inheritance (IS-A) - Appointment IS-A Task
// Time-based event; priority is highest on the day of the event
class Appointment : public Task {
private:
    std::string time;     // e.g. "14:30"
    std::string location;

public:
    // Constructor calls base constructor via initialiser list
    Appointment(int id, const std::string& title, const Date& dueDate,
                const std::string& time, const std::string& location,
                bool completed = false);

    // Getters (Encapsulation)
    std::string getTime()     const { return time; }
    std::string getLocation() const { return location; }

    // Setters (Encapsulation)
    void setTime(const std::string& t)     { time = t; }
    void setLocation(const std::string& l) { location = l; }

    // OOP Concept: Polymorphism - override pure virtuals
    double      getPriorityScore() const override;
    std::string getTypeName()      const override { return "Appointment"; }
    void        displaySummary()   const override;

    // Serialise to pipe-delimited string for file storage
    std::string serialise() const;
};

#endif // APPOINTMENT_H
