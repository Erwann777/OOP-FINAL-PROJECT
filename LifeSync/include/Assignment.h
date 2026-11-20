#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "Task.h"
#include <string>

// OOP Concept: Inheritance (IS-A) - Assignment IS-A Task
// Coursework with a deadline; priority rises as deadline nears AND with higher weight
class Assignment : public Task {
private:
    std::string subject;
    double      weight;   // percentage weight of the coursework (0-100)

public:
    // Constructor calls base constructor via initialiser list
    Assignment(int id, const std::string& title, const Date& dueDate,
               const std::string& subject, double weight, bool completed = false);

    // Getters (Encapsulation)
    std::string getSubject() const { return subject; }
    double      getWeight()  const { return weight; }

    // Setters (Encapsulation)
    void setSubject(const std::string& s) { subject = s; }
    void setWeight(double w);

    // OOP Concept: Polymorphism - override pure virtuals
    double      getPriorityScore() const override;
    std::string getTypeName()      const override { return "Assignment"; }
    void        displaySummary()   const override;

    // Serialise to pipe-delimited string for file storage
    std::string serialise() const;
};

#endif // ASSIGNMENT_H
