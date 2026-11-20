#include "../include/Expense.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Expense::Expense(int id, double amount, const std::string& category,
                 const Date& date, const std::string& note)
    : id(id), amount(amount), category(category), date(date), note(note) {}

void Expense::setAmount(double a) {
    if (a < 0.0)
        throw InvalidInputException("Expense amount cannot be negative.");
    amount = a;
}

void Expense::displaySummary() const {
    std::cout << "[Expense #" << id << "] "
              << std::fixed << std::setprecision(2) << "RM " << amount
              << " | " << category
              << " | " << date
              << " | " << (note.empty() ? "-" : note) << "\n";
}

std::string Expense::serialise() const {
    // Format: id|amount|category|DD/MM/YYYY|note
    std::ostringstream oss;
    oss << id << "|"
        << std::fixed << std::setprecision(2) << amount << "|"
        << category << "|"
        << date.toString() << "|"
        << note;
    return oss.str();
}
