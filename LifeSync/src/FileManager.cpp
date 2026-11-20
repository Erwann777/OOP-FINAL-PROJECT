#include "../include/FileManager.h"
#include "../include/TaskFactory.h"
#include "../include/Assignment.h"
#include "../include/Appointment.h"
#include "../include/Habit.h"
#include "../include/Bill.h"
#include "../include/Exceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

FileManager::FileManager(const std::string& tf, const std::string& ef)
    : taskFile(tf), expenseFile(ef) {}

// Helper: split a line by delimiter
std::vector<std::string> FileManager::split(const std::string& line, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delim)) {
        parts.push_back(token);
    }
    return parts;
}

// Save all data; throws FileException on I/O failure
void FileManager::saveAll(const Planner& planner) const {
    // ---- Save tasks ----
    {
        // Auto-create parent directory if it doesn't exist
        auto taskDir = std::filesystem::path(taskFile).parent_path();
        if (!taskDir.empty())
            std::filesystem::create_directories(taskDir);

        std::ofstream ofs(taskFile);
        if (!ofs.is_open())
            throw FileException("Cannot open " + taskFile + " for writing.");

        for (const auto& t : planner.getTasks()) {
            // OOP Concept: Polymorphism - we need to call serialise on each subclass
            // Use dynamic_cast to call the correct serialise()
            std::string line;
            if (const auto* a = dynamic_cast<const Assignment*>(t.get()))
                line = a->serialise();
            else if (const auto* ap = dynamic_cast<const Appointment*>(t.get()))
                line = ap->serialise();
            else if (const auto* h = dynamic_cast<const Habit*>(t.get()))
                line = h->serialise();
            else if (const auto* b = dynamic_cast<const Bill*>(t.get()))
                line = b->serialise();

            if (!line.empty()) ofs << line << "\n";
        }
    }

    // ---- Save expenses ----
    {
        // Auto-create parent directory if it doesn't exist
        auto expenseDir = std::filesystem::path(expenseFile).parent_path();
        if (!expenseDir.empty())
            std::filesystem::create_directories(expenseDir);

        std::ofstream ofs(expenseFile);
        if (!ofs.is_open())
            throw FileException("Cannot open " + expenseFile + " for writing.");

        for (const auto& e : planner.getExpenses().getAll()) {
            ofs << e.serialise() << "\n";
        }
    }
}

// Load all data; prints warning on error, does NOT throw
void FileManager::loadAll(Planner& planner) const {
    planner.clearAll();
    int maxTaskId    = 0;
    int maxExpenseId = 0;

    // ---- Load tasks ----
    {
        std::ifstream ifs(taskFile);
        if (!ifs.is_open()) {
            std::cout << "  [!] tasks.txt not found - starting with empty task list.\n";
        } else {
            std::string line;
            int lineNum = 0;
            while (std::getline(ifs, line)) {
                lineNum++;
                if (line.empty()) continue;
                try {
                    std::vector<std::string> fields = split(line, '|');
                    if (fields.empty()) continue;
                    std::string type = fields[0];
                    auto task = TaskFactory::create(type, fields);
                    int tid = task->getId();
                    if (tid > maxTaskId) maxTaskId = tid;
                    planner.addTask(std::move(task));
                } catch (const PlannerException& e) {
                    std::cout << "  [!] Skipping corrupted task record (line "
                              << lineNum << "): " << e.what() << "\n";
                } catch (...) {
                    std::cout << "  [!] Skipping unknown error on line " << lineNum << "\n";
                }
            }
        }
    }

    // ---- Load expenses ----
    {
        std::ifstream ifs(expenseFile);
        if (!ifs.is_open()) {
            std::cout << "  [!] expenses.txt not found - starting with empty expense list.\n";
        } else {
            std::string line;
            int lineNum = 0;
            while (std::getline(ifs, line)) {
                lineNum++;
                if (line.empty()) continue;
                try {
                    // Format: id|amount|category|DD/MM/YYYY|note
                    std::vector<std::string> f = split(line, '|');
                    if (f.size() < 5) throw InvalidInputException("Too few fields.");
                    int    id       = std::stoi(f[0]);
                    double amount   = std::stod(f[1]);
                    std::string cat = f[2];
                    Date   date     = Date::parse(f[3]);
                    std::string note = f[4];
                    if (!date.isValid()) throw InvalidInputException("Invalid date.");
                    if (id > maxExpenseId) maxExpenseId = id;
                    planner.addExpense(Expense(id, amount, cat, date, note));
                } catch (const std::exception& e) {
                    std::cout << "  [!] Skipping corrupted expense (line "
                              << lineNum << "): " << e.what() << "\n";
                }
            }
        }
    }

    // Set next ID counters so new items don't duplicate existing IDs
    planner.setNextTaskId(maxTaskId + 1);
    planner.setNextExpenseId(maxExpenseId + 1);
}
