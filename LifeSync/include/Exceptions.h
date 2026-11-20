#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// OOP Concept: Inheritance - custom exception family

// Base exception for all LifeSync errors
class PlannerException : public std::runtime_error {
public:
    explicit PlannerException(const std::string& msg)
        : std::runtime_error(msg) {}
};

// Thrown when user provides invalid input
class InvalidInputException : public PlannerException {
public:
    explicit InvalidInputException(const std::string& msg)
        : PlannerException("Invalid input: " + msg) {}
};

// Thrown when an item is not found
class NotFoundException : public PlannerException {
public:
    explicit NotFoundException(const std::string& msg)
        : PlannerException("Not found: " + msg) {}
};

// Thrown on file I/O failure
class FileException : public PlannerException {
public:
    explicit FileException(const std::string& msg)
        : PlannerException("File error: " + msg) {}
};

#endif // EXCEPTIONS_H
