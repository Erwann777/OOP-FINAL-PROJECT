#ifndef BILL_H
#define BILL_H

#include "Task.h"
#include <string>

// OOP Concept: Inheritance (IS-A) - Bill IS-A Task
// Payment that falls due; priority rises with amount and days overdue
class Bill : public Task {
private:
    double amount; // never negative (validated in setter)
    bool   paid;

public:
    // Constructor calls base constructor via initialiser list
    Bill(int id, const std::string& title, const Date& dueDate,
         double amount, bool paid = false, bool completed = false);

    // Getters (Encapsulation)
    double getAmount() const { return amount; }
    bool   isPaid()    const { return paid; }

    // Setters with validation (Encapsulation)
    void setAmount(double a);   // throws InvalidInputException if a < 0
    void setPaid(bool p)       { paid = p; }

    // OOP Concept: Polymorphism - override pure virtuals
    double      getPriorityScore() const override;
    std::string getTypeName()      const override { return "Bill"; }
    void        displaySummary()   const override;

    // Serialise to pipe-delimited string for file storage
    std::string serialise() const;
};

#endif // BILL_H
