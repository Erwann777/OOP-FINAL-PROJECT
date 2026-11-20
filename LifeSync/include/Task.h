#ifndef TASK_H
#define TASK_H

#include "Date.h"
#include <string>

// OOP Concept: Abstraction - Task is an abstract base class
// Common interface and shared data for every item
class Task {
protected:
    // OOP Concept: Encapsulation - protected members shared with subclasses
    int         id;
    std::string title;
    Date        dueDate;
    bool        completed;

public:
    Task(int id, const std::string& title, const Date& dueDate, bool completed = false);

    // Getters (Encapsulation)
    int         getId()        const { return id; }
    std::string getTitle()     const { return title; }
    Date        getDueDate()   const { return dueDate; }
    bool        isCompleted()  const { return completed; }

    // Setters with validation (Encapsulation)
    void setTitle(const std::string& t);
    void setDueDate(const Date& d);
    void setCompleted(bool c) { completed = c; }

    // OOP Concept: Polymorphism - pure virtual functions
    virtual double      getPriorityScore() const = 0;
    virtual std::string getTypeName()      const = 0;
    virtual void        displaySummary()   const = 0;

    // Virtual destructor for safe polymorphic deletion
    virtual ~Task() = default;
};

#endif // TASK_H
