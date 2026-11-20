#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include "Task.h"
#include <memory>
#include <string>
#include <vector>

// OOP Concept: Factory Design Pattern
// Creates the correct Task subclass from a type string when loading from file
// EXTENSIBILITY: adding a new type only requires one line here + new .h/.cpp
class TaskFactory {
public:
    // Static factory method: parse fields and return the correct Task subclass
    // Format: fields[0] = type, fields[1] = id, fields[2] = title, etc.
    // Throws InvalidInputException if type is unknown or fields are malformed
    static std::unique_ptr<Task> create(const std::string& type,
                                        const std::vector<std::string>& fields);
};

#endif // TASKFACTORY_H
