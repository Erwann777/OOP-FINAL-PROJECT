#ifndef EXPENSE_H
#define EXPENSE_H

#include "Date.h"
#include <string>

// OOP Concept: Encapsulation - Expense is a standalone record (not a Task)
// One spending record: id, amount, category, date, note
class Expense {
private:
    int         id;
    double      amount;
    std::string category;
    Date        date;
    std::string note;

public:
    Expense(int id, double amount, const std::string& category,
            const Date& date, const std::string& note = "");

    // Getters (Encapsulation)
    int         getId()       const { return id; }
    double      getAmount()   const { return amount; }
    std::string getCategory() const { return category; }
    Date        getDate()     const { return date; }
    std::string getNote()     const { return note; }

    // Setters (Encapsulation)
    void setAmount(double a);
    void setCategory(const std::string& c) { category = c; }
    void setNote(const std::string& n)     { note = n; }

    void displaySummary() const;

    // Serialise to pipe-delimited string for file storage
    std::string serialise() const;
};

#endif // EXPENSE_H
