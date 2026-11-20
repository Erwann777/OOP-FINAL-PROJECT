#include "../include/Planner.h"
#include "../include/Bill.h"
#include "../include/Exceptions.h"
#include "../include/Constants.h"
#include <algorithm>
#include <numeric>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>
#include <iomanip>

// OOP Concept: shared_ptr + Move Semantics
// Receives shared ownership of AppConfig; std::move avoids ref-count bump.
Planner::Planner(std::shared_ptr<AppConfig> cfg)
    : config(std::move(cfg)), nextTaskId(1), nextExpenseId(1) {}

// ---- Task operations ----

void Planner::addTask(std::unique_ptr<Task> task) {
    // Check for duplicate ID
    for (const auto& t : tasks) {
        if (t->getId() == task->getId())
            throw InvalidInputException("Duplicate task ID: " + std::to_string(task->getId()));
    }
    tasks.push_back(std::move(task));
}

void Planner::removeTask(int id) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
        [id](const std::unique_ptr<Task>& t) { return t->getId() == id; });
    if (it == tasks.end())
        throw NotFoundException("Task ID " + std::to_string(id));
    tasks.erase(it);
}

Task* Planner::findTaskById(int id) const {
    for (const auto& t : tasks) {
        if (t->getId() == id) return t.get();
    }
    return nullptr;
}

bool Planner::updateTask(int id, const std::string& newTitle, const Date& newDate) {
    Task* t = findTaskById(id);
    if (!t) return false;
    t->setTitle(newTitle);
    t->setDueDate(newDate);
    return true;
}

void Planner::markTaskComplete(int id) {
    Task* t = findTaskById(id);
    if (!t) throw NotFoundException("Task ID " + std::to_string(id));
    t->setCompleted(true);
}

// OOP Concept: Polymorphism - each task calculates its own priority score
void Planner::sortByPriority() {
    std::sort(tasks.begin(), tasks.end(),
        [](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b) {
            return a->getPriorityScore() > b->getPriorityScore();
        });
}

// OOP Concept: Overloading - two search() signatures
std::vector<Task*> Planner::search(const std::string& keyword) const {
    std::vector<Task*> results;
    std::string kw = keyword;
    // Case-insensitive: convert keyword to lower
    std::transform(kw.begin(), kw.end(), kw.begin(), ::tolower);

    for (const auto& t : tasks) {
        std::string title = t->getTitle();
        std::transform(title.begin(), title.end(), title.begin(), ::tolower);
        if (title.find(kw) != std::string::npos)
            results.push_back(t.get());
    }
    return results;
}

std::vector<Task*> Planner::search(const Date& from, const Date& to) const {
    std::vector<Task*> results;
    for (const auto& t : tasks) {
        const Date& d = t->getDueDate();
        if (from <= d && d <= to)
            results.push_back(t.get());
    }
    return results;
}

std::vector<Task*> Planner::filterByType(const std::string& typeName) const {
    std::vector<Task*> results;
    for (const auto& t : tasks) {
        if (t->getTypeName() == typeName)
            results.push_back(t.get());
    }
    return results;
}

std::vector<Task*> Planner::filterByStatus(bool completed) const {
    std::vector<Task*> results;
    for (const auto& t : tasks) {
        if (t->isCompleted() == completed)
            results.push_back(t.get());
    }
    return results;
}

// ---- Dashboard helpers ----

std::vector<Task*> Planner::getTodayItems() const {
    Date today = Date::today();
    std::vector<Task*> results;
    for (const auto& t : tasks) {
        if (t->getDueDate() == today && !t->isCompleted())
            results.push_back(t.get());
    }
    return results;
}

std::vector<Task*> Planner::getOverdueItems() const {
    Date today = Date::today();
    std::vector<Task*> results;
    for (const auto& t : tasks) {
        if (t->getDueDate() < today && !t->isCompleted())
            results.push_back(t.get());
    }
    return results;
}

std::vector<Task*> Planner::getNext7Days() const {
    Date today = Date::today();
    std::vector<Task*> results;
    for (const auto& t : tasks) {
        int days = t->getDueDate().daysUntil();
        if (days > 0 && days <= 7 && !t->isCompleted())
            results.push_back(t.get());
    }
    return results;
}

// ---- Expense operations ----

void Planner::addExpense(const Expense& e) {
    expenses.add(e);
}

void Planner::removeExpense(int id) {
    expenses.remove(id);  // Repository::remove throws NotFoundException
}

Expense* Planner::findExpenseById(int id) {
    return expenses.findById(id);
}

// ---- Expense report ----
void Planner::expenseReport() const {
    const auto& all = expenses.getAll();
    if (all.empty()) {
        std::cout << "  No expenses recorded.\n";
        return;
    }

    const std::string& sym = config->currencySymbol;

    // OOP Concept: unordered_map — O(1) average-case insertion/lookup per category
    std::unordered_map<std::string, double> catTotals;
    for (const auto& e : all) {
        catTotals[e.getCategory()] += e.getAmount();
    }

    // Copy into std::map for sorted (alphabetical) output
    std::map<std::string, double> sortedTotals(catTotals.begin(), catTotals.end());

    std::cout << "\n  === Expense Report by Category ===\n";
    for (const auto& kv : sortedTotals) {
        std::cout << "  " << std::left << std::setw(20) << kv.first
                  << " " << sym << " "
                  << std::fixed << std::setprecision(2) << kv.second << "\n";
    }

    // Largest expense using std::max_element + lambda
    auto maxIt = std::max_element(all.begin(), all.end(),
        [](const Expense& a, const Expense& b) {
            return a.getAmount() < b.getAmount();
        });
    std::cout << "\n  Largest expense: " << sym << " "
              << std::fixed << std::setprecision(2) << maxIt->getAmount()
              << " (" << maxIt->getCategory() << " - " << maxIt->getNote() << ")\n";

    // Grand total using std::accumulate + lambda
    double total = std::accumulate(all.begin(), all.end(), 0.0,
        [](double sum, const Expense& e) { return sum + e.getAmount(); });
    std::cout << "  Grand total     : " << sym << " "
              << std::fixed << std::setprecision(2) << total << "\n";
}

// ---- Bill helpers ----
double Planner::totalUnpaidBills() const {
    double total = 0.0;
    for (const auto& t : tasks) {
        // OOP Concept: safe downcast using dynamic_cast
        const Bill* b = dynamic_cast<const Bill*>(t.get());
        if (b && !b->isPaid()) total += b->getAmount();
    }
    return total;
}

// ---- Summary report ----
void Planner::summaryReport() const {
    int total     = static_cast<int>(tasks.size());
    int completed = static_cast<int>(std::count_if(tasks.begin(), tasks.end(),
        [](const std::unique_ptr<Task>& t) { return t->isCompleted(); }));
    int pending   = total - completed;

    const auto& expAll = expenses.getAll();
    double totalSpending = std::accumulate(expAll.begin(), expAll.end(), 0.0,
        [](double s, const Expense& e) { return s + e.getAmount(); });

    const std::string& sym = config->currencySymbol;

    std::cout << "\n  === Summary Report ===\n"
              << "  Total tasks    : " << total     << "\n"
              << "  Completed      : " << completed << "\n"
              << "  Pending        : " << pending   << "\n"
              << "  Total spending : " << sym << " "
              << std::fixed << std::setprecision(2) << totalSpending << "\n"
              << "  Unpaid bills   : " << sym << " "
              << std::fixed << std::setprecision(2) << totalUnpaidBills() << "\n";

    // OOP Concept: STL any_of / all_of with lambda — status insight
    if (allTasksCompleted())
        std::cout << "  [*] All tasks completed! Great job.\n";
    else if (hasOverdueItems())
        std::cout << "  [!] Warning: you have overdue items - check the dashboard.\n";
}

void Planner::clearAll() {
    tasks.clear();
    expenses.clear();
    nextTaskId    = 1;
    nextExpenseId = 1;
}

// OOP Concept: STL set — returns all distinct expense category strings
std::set<std::string> Planner::getUniqueCategories() const {
    std::set<std::string> cats;
    for (const auto& e : expenses.getAll()) {
        cats.insert(e.getCategory());
    }
    return cats;
}

// OOP Concept: STL any_of — true if at least one incomplete task is past its due date
bool Planner::hasOverdueItems() const {
    return std::any_of(tasks.begin(), tasks.end(),
        [](const std::unique_ptr<Task>& t) {
            return !t->isCompleted() && t->getDueDate() < Date::today();
        });
}

// OOP Concept: STL all_of — true only when every task has been marked complete
bool Planner::allTasksCompleted() const {
    if (tasks.empty()) return false;
    return std::all_of(tasks.begin(), tasks.end(),
        [](const std::unique_ptr<Task>& t) { return t->isCompleted(); });
}
