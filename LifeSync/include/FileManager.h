#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Planner.h"
#include <string>

// OOP Concept: Encapsulation - FileManager hides all file I/O details
// Saves and loads all planner data; throws FileException on failure
class FileManager {
private:
    std::string taskFile;
    std::string expenseFile;

    // Helper: split a pipe-delimited line into fields
    static std::vector<std::string> split(const std::string& line, char delim = '|');

public:
    FileManager(const std::string& taskFile    = "data/tasks.txt",
                const std::string& expenseFile = "data/expenses.txt");

    // Save all tasks and expenses to their respective files
    // Throws FileException on failure
    void saveAll(const Planner& planner) const;

    // Load all tasks and expenses from files into the planner
    // Prints a warning message if a file is missing or corrupted; does NOT throw
    void loadAll(Planner& planner) const;
};

#endif // FILEMANAGER_H
