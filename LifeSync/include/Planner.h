#ifndef PLANNER_H
#define PLANNER_H

#include "Task.h"
#include "Expense.h"
#include "Repository.h"
#include "AppConfig.h"
#include "Constants.h"
#include <vector>
#include <memory>
#include <set>
#include <string>

// OOP Concept: Composition - Planner OWNS tasks and expenses
// Manager class that owns all data
class Planner {
private:
    // OOP Concept: Shared ownership (shared_ptr) — AppConfig is shared with Menu;
    // a single instance in main() is owned jointly by both objects.
    std::shared_ptr<AppConfig> config;

    // OOP Concept: Smart pointers - unique_ptr<Task> for polymorphic ownership
    std::vector<std::unique_ptr<Task>> tasks;

    // OOP Concept: Composition - Planner owns a Repository of Expenses
    Repository<Expense> expenses;

    int nextTaskId;
    int nextExpenseId;

public:
    // OOP Concept: shared_ptr — receives shared AppConfig (default = brand-new instance)
    explicit Planner(std::shared_ptr<AppConfig> cfg = AppConfig::makeDefault());

    // ---- Task operations ----
    void addTask(std::unique_ptr<Task> task);
    void removeTask(int id);
    Task* findTaskById(int id) const;

    // Update fields of an existing task; returns false if not found
    bool updateTask(int id, const std::string& newTitle, const Date& newDate);

    void markTaskComplete(int id);

    // OOP Concept: Polymorphism - loops over Task* pointers, each runs its own priority
    void sortByPriority();

    const std::vector<std::unique_ptr<Task>>& getTasks() const { return tasks; }

    // OOP Concept: Overloading - two search signatures
    std::vector<Task*> search(const std::string& keyword) const;
    std::vector<Task*> search(const Date& from, const Date& to) const;

    // Filter by type name
    std::vector<Task*> filterByType(const std::string& typeName) const;

    // Filter by completion status
    std::vector<Task*> filterByStatus(bool completed) const;

    // Dashboard helpers
    std::vector<Task*> getTodayItems()   const;
    std::vector<Task*> getOverdueItems() const;
    std::vector<Task*> getNext7Days()    const;

    // ---- Expense operations ----
    void    addExpense(const Expense& e);
    void    removeExpense(int id);
    Expense* findExpenseById(int id);

    const Repository<Expense>& getExpenses() const { return expenses; }
    Repository<Expense>&       getExpenses()       { return expenses; }

    // Monthly expense report: total per category, largest expense
    void expenseReport() const;

    // Bill helpers
    double totalUnpaidBills() const;

    // Summary report
    void summaryReport() const;

    // ID generators
    int getNextTaskId()    { return nextTaskId++; }
    int getNextExpenseId() { return nextExpenseId++; }

    // Used by FileManager to rebuild planner from loaded data
    void setNextTaskId(int id)    { nextTaskId = id; }
    void setNextExpenseId(int id) { nextExpenseId = id; }
    void clearAll();

    // OOP Concept: STL set — returns all distinct expense category names
    std::set<std::string> getUniqueCategories() const;

    // OOP Concept: STL any_of / all_of — boolean status helpers
    bool hasOverdueItems()   const;  // true if at least one incomplete task is overdue
    bool allTasksCompleted() const;  // true if every task is marked complete
};

#endif // PLANNER_H
