#include "../include/Bill.h"
#include "../include/Exceptions.h"
#include "../include/Constants.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

// OOP Concept: Inheritance - constructor delegates to base via initialiser list
Bill::Bill(int id, const std::string& title, const Date& dueDate,
           double amount, bool paid, bool completed)
    : Task(id, title, dueDate, completed), amount(amount), paid(paid) {}

// OOP Concept: Encapsulation - amount can NEVER be set negative
void Bill::setAmount(double a) {
    if (a < 0.0)
        throw InvalidInputException("Bill amount cannot be negative.");
    amount = a;
}

// OOP Concept: Polymorphism - priority rises with amount and days overdue
double Bill::getPriorityScore() const {
    if (completed || paid) return 0.0;

    int days = dueDate.daysUntil(); // negative = overdue

    double urgency = 0.0;
    if (days < 0)        urgency = 100.0 + std::abs(days) * 2.0; // escalates when overdue
    else if (days == 0)  urgency = 95.0;
    else if (days <= 3)  urgency = 70.0;
    else if (days <= 7)  urgency = 50.0;
    else                 urgency = 25.0;

    // Amount factor: larger bills get extra priority (capped at LifeSync::MAX_AMOUNT_FACTOR)
    double amountFactor = std::min(amount / 10.0, LifeSync::MAX_AMOUNT_FACTOR);

    return urgency + amountFactor;
}

void Bill::displaySummary() const {
    std::cout << "[Bill #" << id << "] " << title << "\n"
              << "  Amount  : RM " << std::fixed << std::setprecision(2) << amount << "\n"
              << "  Due     : " << dueDate
              << " | Paid: " << (paid ? "Yes" : "No") << "\n"
              << "  Status  : " << (completed ? "Done" : "Pending") << "\n"
              << "  Priority: " << getPriorityScore() << "\n";
}

std::string Bill::serialise() const {
    // Format: Bill|id|title|DD/MM/YYYY|completed|amount|paid
    std::ostringstream oss;
    oss << "Bill|" << id << "|" << title << "|"
        << dueDate.toString() << "|" << completed << "|"
        << std::fixed << std::setprecision(2) << amount << "|" << paid;
    return oss.str();
}
