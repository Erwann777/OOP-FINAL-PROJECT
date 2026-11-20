#ifndef MENU_H
#define MENU_H

#include "Planner.h"
#include "FileManager.h"
#include "AppConfig.h"
#include <memory>

// OOP Concept: Encapsulation - Menu is the ONLY class using cin/cout
// Main menu loop, sub-menus, display formatting
class Menu {
private:
    Planner&     planner;
    FileManager& fileManager;
    // OOP Concept: shared_ptr — same AppConfig instance as Planner (shared ownership)
    std::shared_ptr<AppConfig> config;

    // ---- Display helpers ----
    static void printSeparator(char c = '-', int width = 60);
    static void printHeader(const std::string& title);

    // ---- Sub-menus ----
    void taskMenu();
    void assignmentMenu();
    void appointmentMenu();
    void habitMenu();
    void billMenu();
    void expenseMenu();
    void searchMenu();
    void reportMenu();

    // ---- Task CRUD ----
    void addAssignment();
    void addAppointment();
    void addHabit();
    void addBill();
    void viewAllTasks();
    void viewTasksByType();
    void updateTask();
    void deleteTask();
    void markComplete();

    // ---- Habit specific ----
    void recordHabitCompletion();

    // ---- Expense CRUD ----
    void addExpense();
    void viewExpenses();
    void deleteExpense();

    // ---- Search ----
    void searchByKeyword();
    void searchByDateRange();
    void searchByType();
    void searchByStatus();

    // ---- Reports ----
    void showDashboard();
    void showSummaryReport();
    void showExpenseReport();
    void showBillReminders();

    // Save after every change
    void autoSave();

public:
    Menu(Planner& planner, FileManager& fileManager,
         std::shared_ptr<AppConfig> cfg = AppConfig::makeDefault());

    // Launch the main menu loop; returns when user exits
    void run();
};

#endif // MENU_H
