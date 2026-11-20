#include "../include/Task.h"
#include "../include/Exceptions.h"

// OOP Concept: Encapsulation - constructor initialises protected data
Task::Task(int id, const std::string& title, const Date& dueDate, bool completed)
    : id(id), title(title), dueDate(dueDate), completed(completed) {}

void Task::setTitle(const std::string& t) {
    if (t.empty()) throw InvalidInputException("Title cannot be empty.");
    title = t;
}

void Task::setDueDate(const Date& d) {
    if (!d.isValid()) throw InvalidInputException("Date is not valid.");
    dueDate = d;
}
