#include "../include/TaskFactory.h"
#include "../include/Assignment.h"
#include "../include/Appointment.h"
#include "../include/Habit.h"
#include "../include/Bill.h"
#include "../include/Exceptions.h"
#include <stdexcept>

// OOP Concept: Factory Design Pattern
// Creates the correct Task subclass based on the type string from saved file
std::unique_ptr<Task> TaskFactory::create(const std::string& type,
                                          const std::vector<std::string>& fields) {
    try {
        if (type == "Assignment") {
            // fields: [type, id, title, date, completed, subject, weight]
            if (fields.size() < 7)
                throw InvalidInputException("Assignment record has too few fields.");
            int    id        = std::stoi(fields[1]);
            std::string title  = fields[2];
            Date   dueDate   = Date::parse(fields[3]);
            bool   completed = (fields[4] == "1");
            std::string subject = fields[5];
            double weight    = std::stod(fields[6]);
            if (!dueDate.isValid())
                throw InvalidInputException("Invalid date in Assignment record.");
            return std::make_unique<Assignment>(id, title, dueDate, subject, weight, completed);
        }

        if (type == "Appointment") {
            // fields: [type, id, title, date, completed, time, location]
            if (fields.size() < 7)
                throw InvalidInputException("Appointment record has too few fields.");
            int    id       = std::stoi(fields[1]);
            std::string title = fields[2];
            Date   dueDate  = Date::parse(fields[3]);
            bool completed  = (fields[4] == "1");
            std::string time     = fields[5];
            std::string location = fields[6];
            if (!dueDate.isValid())
                throw InvalidInputException("Invalid date in Appointment record.");
            return std::make_unique<Appointment>(id, title, dueDate, time, location, completed);
        }

        if (type == "Habit") {
            // fields: [type, id, title, date, completed, currentStreak, bestStreak]
            if (fields.size() < 7)
                throw InvalidInputException("Habit record has too few fields.");
            int    id            = std::stoi(fields[1]);
            std::string title      = fields[2];
            Date   dueDate       = Date::parse(fields[3]);
            bool   completed     = (fields[4] == "1");
            int    currentStreak = std::stoi(fields[5]);
            int    bestStreak    = std::stoi(fields[6]);
            if (!dueDate.isValid())
                throw InvalidInputException("Invalid date in Habit record.");
            return std::make_unique<Habit>(id, title, dueDate, currentStreak, bestStreak, completed);
        }

        if (type == "Bill") {
            // fields: [type, id, title, date, completed, amount, paid]
            if (fields.size() < 7)
                throw InvalidInputException("Bill record has too few fields.");
            int    id        = std::stoi(fields[1]);
            std::string title  = fields[2];
            Date   dueDate   = Date::parse(fields[3]);
            bool   completed = (fields[4] == "1");
            double amount    = std::stod(fields[5]);
            bool   paid      = (fields[6] == "1");
            if (!dueDate.isValid())
                throw InvalidInputException("Invalid date in Bill record.");
            return std::make_unique<Bill>(id, title, dueDate, amount, paid, completed);
        }

        // EXTENSIBILITY: add new types here - one line per new Task subclass
        throw InvalidInputException("Unknown task type: " + type);

    } catch (const InvalidInputException&) {
        throw; // re-throw our own exceptions
    } catch (const std::exception& e) {
        throw InvalidInputException(std::string("Malformed record: ") + e.what());
    }
}
