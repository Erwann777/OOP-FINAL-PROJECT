#include "../include/Menu.h"
#include "../include/InputValidator.h"
#include "../include/Constants.h"
#include "../include/Assignment.h"
#include "../include/Appointment.h"
#include "../include/Habit.h"
#include "../include/Bill.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

// OOP Concept: Composition - Menu uses Planner and FileManager via reference

// OOP Concept: shared_ptr + Move Semantics — Menu receives shared AppConfig ownership
Menu::Menu(Planner& planner, FileManager& fileManager, std::shared_ptr<AppConfig> cfg)
    : planner(planner), fileManager(fileManager), config(std::move(cfg)) {}

// ---- Static display helpers ----

void Menu::printSeparator(char c, int width) {
    std::cout << std::string(width, c) << "\n";
}

void Menu::printHeader(const std::string& title) {
    printSeparator('=');
    std::cout << "  " << title << "\n";
    printSeparator('=');
}

// ---- Auto-save after every change ----
void Menu::autoSave() {
    try {
        fileManager.saveAll(planner);
    } catch (const FileException& e) {
        std::cout << "  [!] Auto-save failed: " << e.what() << "\n";
    }
}

// =============================================================
//  MAIN MENU LOOP
// =============================================================
void Menu::run() {
    std::cout << "\n";
    printSeparator('*');
    std::cout << "  Welcome to LifeSync - Your Personal Life Manager\n";
    printSeparator('*');

    while (true) {
        std::cout << "\n";
        printHeader("MAIN MENU");
        std::cout << "  [1] Task Management\n"
                  << "  [2] Expense Tracker\n"
                  << "  [3] Search & Filter\n"
                  << "  [4] Dashboard & Reports\n"
                  << "  [0] Exit\n";
        printSeparator();

        int choice = InputValidator::readInt("  Your choice: ", 0, 4);
        switch (choice) {
            case 1: taskMenu();    break;
            case 2: expenseMenu(); break;
            case 3: searchMenu();  break;
            case 4: reportMenu();  break;
            case 0:
                std::cout << "\n  Saving data...\n";
                autoSave();
                std::cout << "  Goodbye!\n\n";
                return;
        }
    }
}

// =============================================================
//  TASK MENU
// =============================================================
void Menu::taskMenu() {
    while (true) {
        std::cout << "\n";
        printHeader("TASK MANAGEMENT");
        std::cout << "  [1] Add Assignment\n"
                  << "  [2] Add Appointment\n"
                  << "  [3] Add Habit\n"
                  << "  [4] Add Bill\n"
                  << "  [5] View All Tasks (sorted by priority)\n"
                  << "  [6] View Tasks by Type\n"
                  << "  [7] Update Task\n"
                  << "  [8] Delete Task\n"
                  << "  [9] Mark Task Complete\n"
                  << " [10] Record Habit Completion\n"
                  << "  [0] Back\n";
        printSeparator();

        int choice = InputValidator::readInt("  Your choice: ", 0, 10);
        switch (choice) {
            case 1: addAssignment();         break;
            case 2: addAppointment();        break;
            case 3: addHabit();              break;
            case 4: addBill();               break;
            case 5: viewAllTasks();          break;
            case 6: viewTasksByType();       break;
            case 7: updateTask();            break;
            case 8: deleteTask();            break;
            case 9: markComplete();          break;
            case 10: recordHabitCompletion(); break;
            case 0: return;
        }
    }
}

// ---- Add Assignment ----
void Menu::addAssignment() {
    printHeader("ADD ASSIGNMENT");
    std::string title   = InputValidator::readNonEmptyString("  Title   : ");
    Date dueDate        = InputValidator::readDate("  Due date (DD/MM/YYYY): ");
    std::string subject = InputValidator::readNonEmptyString("  Subject : ");
    double weight       = InputValidator::readDouble("  Weight (0-100): ", 0.0);
    // OOP Concept: constexpr from LifeSync namespace
    if (weight > LifeSync::MAX_WEIGHT) {
        std::cout << "  [!] Weight capped at " << LifeSync::MAX_WEIGHT << ".\n";
        weight = LifeSync::MAX_WEIGHT;
    }

    int id = planner.getNextTaskId();
    try {
        planner.addTask(std::make_unique<Assignment>(id, title, dueDate, subject, weight));
        std::cout << "  [+] Assignment added (ID " << id << ").\n";
        autoSave();
    } catch (const PlannerException& e) {
        std::cout << "  [!] " << e.what() << "\n";
    }
}

// ---- Add Appointment ----
void Menu::addAppointment() {
    printHeader("ADD APPOINTMENT");
    std::string title    = InputValidator::readNonEmptyString("  Title   : ");
    Date dueDate         = InputValidator::readDate("  Date (DD/MM/YYYY): ");
    std::string time     = InputValidator::readNonEmptyString("  Time (e.g. 14:30): ");
    std::string location = InputValidator::readNonEmptyString("  Location: ");

    int id = planner.getNextTaskId();
    try {
        planner.addTask(std::make_unique<Appointment>(id, title, dueDate, time, location));
        std::cout << "  [+] Appointment added (ID " << id << ").\n";
        autoSave();
    } catch (const PlannerException& e) {
        std::cout << "  [!] " << e.what() << "\n";
    }
}

// ---- Add Habit ----
void Menu::addHabit() {
    printHeader("ADD HABIT");
    std::string title = InputValidator::readNonEmptyString("  Title   : ");
    Date dueDate      = InputValidator::readDate("  Start date (DD/MM/YYYY): ");

    int id = planner.getNextTaskId();
    try {
        planner.addTask(std::make_unique<Habit>(id, title, dueDate));
        std::cout << "  [+] Habit added (ID " << id << ").\n";
        autoSave();
    } catch (const PlannerException& e) {
        std::cout << "  [!] " << e.what() << "\n";
    }
}

// ---- Add Bill ----
void Menu::addBill() {
    printHeader("ADD BILL");
    std::string title = InputValidator::readNonEmptyString("  Title   : ");
    Date dueDate      = InputValidator::readDate("  Due date (DD/MM/YYYY): ");
    double amount     = InputValidator::readDouble("  Amount (" + config->currencySymbol + "): ", 0.0);

    int id = planner.getNextTaskId();
    try {
        planner.addTask(std::make_unique<Bill>(id, title, dueDate, amount));
        std::cout << "  [+] Bill added (ID " << id << ").\n";
        autoSave();
    } catch (const PlannerException& e) {
        std::cout << "  [!] " << e.what() << "\n";
    }
}

// ---- View all tasks sorted by priority ----
void Menu::viewAllTasks() {
    printHeader("ALL TASKS (sorted by priority)");
    if (planner.getTasks().empty()) {
        std::cout << "  No tasks found.\n";
        return;
    }
    planner.sortByPriority();
    for (const auto& t : planner.getTasks()) {
        // OOP Concept: Polymorphism - calls the correct displaySummary()
        t->displaySummary();
        printSeparator('-', 50);
    }
}

// ---- View tasks by type ----
void Menu::viewTasksByType() {
    printHeader("VIEW BY TYPE");
    std::cout << "  [1] Assignments\n"
              << "  [2] Appointments\n"
              << "  [3] Habits\n"
              << "  [4] Bills\n";
    int choice = InputValidator::readInt("  Choice: ", 1, 4);
    std::string types[] = {"Assignment", "Appointment", "Habit", "Bill"};
    std::string typeName = types[choice - 1];

    auto results = planner.filterByType(typeName);
    if (results.empty()) {
        std::cout << "  No " << typeName << "s found.\n";
        return;
    }
    for (auto* t : results) {
        t->displaySummary();
        printSeparator('-', 50);
    }
}

// ---- Update task ----
void Menu::updateTask() {
    printHeader("UPDATE TASK");
    int id = InputValidator::readInt("  Task ID: ", 1);
    Task* t = planner.findTaskById(id);
    if (!t) {
        std::cout << "  [!] Task not found.\n";
        return;
    }
    t->displaySummary();

    // Allow blank input to keep the current value
    std::string newTitle = InputValidator::readString("  New title (blank = keep current): ");
    if (newTitle.empty()) newTitle = t->getTitle();

    std::string newDateStr = InputValidator::readString("  New due date DD/MM/YYYY (blank = keep current): ");
    Date newDate;
    if (newDateStr.empty()) {
        newDate = t->getDueDate();
    } else {
        newDate = Date::parse(newDateStr);
        while (!newDate.isValid()) {
            std::cout << "  [!] Invalid date. Use DD/MM/YYYY (e.g. 25/12/2025).\n";
            newDateStr = InputValidator::readString("  New due date DD/MM/YYYY (blank = keep current): ");
            if (newDateStr.empty()) { newDate = t->getDueDate(); break; }
            newDate = Date::parse(newDateStr);
        }
    }

    try {
        planner.updateTask(id, newTitle, newDate);
        std::cout << "  [+] Task updated.\n";
        autoSave();
    } catch (const PlannerException& e) {
        std::cout << "  [!] " << e.what() << "\n";
    }
}

// ---- Delete task ----
void Menu::deleteTask() {
    printHeader("DELETE TASK");
    int id = InputValidator::readInt("  Task ID to delete: ", 1);
    try {
        planner.removeTask(id);
        std::cout << "  [-] Task deleted.\n";
        autoSave();
    } catch (const NotFoundException& e) {
        std::cout << "  [!] " << e.what() << "\n";
    }
}

// ---- Mark complete ----
void Menu::markComplete() {
    printHeader("MARK TASK COMPLETE");
    int id = InputValidator::readInt("  Task ID: ", 1);
    try {
        planner.markTaskComplete(id);

        // If it's a Bill, mark as paid too
        Task* t = planner.findTaskById(id);
        if (t) {
            Bill* b = dynamic_cast<Bill*>(t);
            if (b) b->setPaid(true);
        }

        std::cout << "  [+] Task marked as complete.\n";
        autoSave();
    } catch (const NotFoundException& e) {
        std::cout << "  [!] " << e.what() << "\n";
    }
}

// ---- Record habit completion ----
void Menu::recordHabitCompletion() {
    printHeader("RECORD HABIT COMPLETION");
    int id = InputValidator::readInt("  Habit ID: ", 1);
    Task* t = planner.findTaskById(id);
    if (!t || t->getTypeName() != "Habit") {
        std::cout << "  [!] Habit not found.\n";
        return;
    }
    Habit* h = dynamic_cast<Habit*>(t);
    h->recordCompletion();
    std::cout << "  [+] Habit recorded! Streak: " << h->getCurrentStreak()
              << " days (best: " << h->getBestStreak() << ").\n";
    autoSave();
}

// =============================================================
//  EXPENSE MENU
// =============================================================
void Menu::expenseMenu() {
    while (true) {
        std::cout << "\n";
        printHeader("EXPENSE TRACKER");
        std::cout << "  [1] Add Expense\n"
                  << "  [2] View All Expenses\n"
                  << "  [3] Delete Expense\n"
                  << "  [0] Back\n";
        printSeparator();

        int choice = InputValidator::readInt("  Your choice: ", 0, 3);
        switch (choice) {
            case 1: addExpense();    break;
            case 2: viewExpenses();  break;
            case 3: deleteExpense(); break;
            case 0: return;
        }
    }
}

void Menu::addExpense() {
    printHeader("ADD EXPENSE");
    double amount       = InputValidator::readDouble("  Amount (" + config->currencySymbol + "): ", 0.01);
    std::string category = InputValidator::readNonEmptyString("  Category   : ");
    Date date           = InputValidator::readDate("  Date (DD/MM/YYYY): ");
    std::string note    = InputValidator::readString("  Note (optional): ");

    int id = planner.getNextExpenseId();
    planner.addExpense(Expense(id, amount, category, date, note));
    std::cout << "  [+] Expense added (ID " << id << ").\n";
    autoSave();
}

void Menu::viewExpenses() {
    printHeader("ALL EXPENSES");
    const auto& all = planner.getExpenses().getAll();
    if (all.empty()) {
        std::cout << "  No expenses recorded.\n";
        return;
    }
    for (const auto& e : all) {
        e.displaySummary();
    }
}

void Menu::deleteExpense() {
    printHeader("DELETE EXPENSE");
    int id = InputValidator::readInt("  Expense ID: ", 1);
    try {
        planner.removeExpense(id);
        std::cout << "  [-] Expense deleted.\n";
        autoSave();
    } catch (const NotFoundException& e) {
        std::cout << "  [!] " << e.what() << "\n";
    }
}

// =============================================================
//  SEARCH MENU
// =============================================================
void Menu::searchMenu() {
    while (true) {
        std::cout << "\n";
        printHeader("SEARCH & FILTER");
        std::cout << "  [1] Search by keyword\n"
                  << "  [2] Search by date range\n"
                  << "  [3] Filter by type\n"
                  << "  [4] Filter by completion status\n"
                  << "  [0] Back\n";
        printSeparator();

        int choice = InputValidator::readInt("  Your choice: ", 0, 4);
        switch (choice) {
            case 1: searchByKeyword();   break;
            case 2: searchByDateRange(); break;
            case 3: searchByType();      break;
            case 4: searchByStatus();    break;
            case 0: return;
        }
    }
}

// OOP Concept: Overloading - search(keyword) vs search(from,to)
void Menu::searchByKeyword() {
    printHeader("SEARCH BY KEYWORD");
    std::string kw = InputValidator::readNonEmptyString("  Keyword: ");
    auto results = planner.search(kw);
    if (results.empty()) {
        std::cout << "  No tasks match '" << kw << "'.\n";
        return;
    }
    std::cout << "  Found " << results.size() << " result(s):\n";
    for (auto* t : results) {
        t->displaySummary();
        printSeparator('-', 50);
    }
}

void Menu::searchByDateRange() {
    printHeader("SEARCH BY DATE RANGE");
    Date from = InputValidator::readDate("  From (DD/MM/YYYY): ");
    Date to   = InputValidator::readDate("  To   (DD/MM/YYYY): ");
    if (to < from) {
        std::cout << "  [!] 'To' date must be after 'From' date.\n";
        return;
    }
    auto results = planner.search(from, to);
    if (results.empty()) {
        std::cout << "  No tasks in that date range.\n";
        return;
    }
    std::cout << "  Found " << results.size() << " result(s):\n";
    for (auto* t : results) {
        t->displaySummary();
        printSeparator('-', 50);
    }
}

void Menu::searchByType() {
    printHeader("FILTER BY TYPE");
    std::cout << "  [1] Assignment  [2] Appointment  [3] Habit  [4] Bill\n";
    int choice = InputValidator::readInt("  Choice: ", 1, 4);
    std::string types[] = {"Assignment", "Appointment", "Habit", "Bill"};
    auto results = planner.filterByType(types[choice - 1]);
    if (results.empty()) {
        std::cout << "  No " << types[choice - 1] << "s found.\n";
        return;
    }
    for (auto* t : results) {
        t->displaySummary();
        printSeparator('-', 50);
    }
}

void Menu::searchByStatus() {
    printHeader("FILTER BY STATUS");
    std::cout << "  [1] Pending  [2] Completed\n";
    int choice = InputValidator::readInt("  Choice: ", 1, 2);
    bool wantCompleted = (choice == 2);
    auto results = planner.filterByStatus(wantCompleted);
    if (results.empty()) {
        std::cout << "  No " << (wantCompleted ? "completed" : "pending") << " tasks.\n";
        return;
    }
    for (auto* t : results) {
        t->displaySummary();
        printSeparator('-', 50);
    }
}

// =============================================================
//  REPORT MENU
// =============================================================
void Menu::reportMenu() {
    while (true) {
        std::cout << "\n";
        printHeader("DASHBOARD & REPORTS");
        std::cout << "  [1] Today's Dashboard\n"
                  << "  [2] Summary Report\n"
                  << "  [3] Expense Report\n"
                  << "  [4] Bill Reminders\n"
                  << "  [0] Back\n";
        printSeparator();

        int choice = InputValidator::readInt("  Your choice: ", 0, 4);
        switch (choice) {
            case 1: showDashboard();     break;
            case 2: showSummaryReport(); break;
            case 3: showExpenseReport(); break;
            case 4: showBillReminders(); break;
            case 0: return;
        }
    }
}

void Menu::showDashboard() {
    printHeader("TODAY'S DASHBOARD");
    Date today = Date::today();
    std::cout << "  Date: " << today << "\n\n";

    auto todayItems   = planner.getTodayItems();
    auto overdueItems = planner.getOverdueItems();
    auto next7Items   = planner.getNext7Days();

    std::cout << "  --- TODAY'S ITEMS (" << todayItems.size() << ") ---\n";
    if (todayItems.empty()) std::cout << "  Nothing due today.\n";
    for (auto* t : todayItems) {
        std::cout << "  [" << t->getTypeName() << "] " << t->getTitle() << "\n";
    }

    std::cout << "\n  --- OVERDUE ITEMS (" << overdueItems.size() << ") ---\n";
    if (overdueItems.empty()) std::cout << "  No overdue items.\n";
    for (auto* t : overdueItems) {
        std::cout << "  [" << t->getTypeName() << "] " << t->getTitle()
                  << " (due " << t->getDueDate() << ")\n";
    }

    std::cout << "\n  --- NEXT 7 DAYS (" << next7Items.size() << ") ---\n";
    if (next7Items.empty()) std::cout << "  Nothing in the next 7 days.\n";
    for (auto* t : next7Items) {
        std::cout << "  [" << t->getTypeName() << "] " << t->getTitle()
                  << " (due " << t->getDueDate() << ")\n";
    }
    std::cout << "\n";
}

void Menu::showSummaryReport() {
    printHeader("SUMMARY REPORT");
    planner.summaryReport();
}

void Menu::showExpenseReport() {
    printHeader("EXPENSE REPORT");
    // OOP Concept: std::set — display unique expense categories before full report
    auto cats = planner.getUniqueCategories();
    if (!cats.empty()) {
        std::cout << "  Expense categories: ";
        for (const auto& c : cats) std::cout << "[" << c << "] ";
        std::cout << "\n\n";
    }
    planner.expenseReport();
}

void Menu::showBillReminders() {
    printHeader("BILL REMINDERS");
    auto bills = planner.filterByType("Bill");
    if (bills.empty()) {
        std::cout << "  No bills found.\n";
        return;
    }

    double unpaidTotal = 0.0;
    std::cout << "  Unpaid / Upcoming Bills:\n";
    for (auto* t : bills) {
        Bill* b = dynamic_cast<Bill*>(t);
        if (!b || b->isPaid()) continue;

        int days = b->getDueDate().daysUntil();
        unpaidTotal += b->getAmount();

        std::string status;
        if (days < 0)      status = "OVERDUE by " + std::to_string(-days) + " day(s)!";
        else if (days == 0) status = "DUE TODAY!";
        else if (days <= 7) status = "Due in " + std::to_string(days) + " day(s)";
        else                status = "Due " + b->getDueDate().toString();

        std::cout << "  [Bill #" << b->getId() << "] " << b->getTitle()
                  << " - " << config->currencySymbol << " "
                  << std::fixed << std::setprecision(2) << b->getAmount()
                  << " | " << status << "\n";
    }
    std::cout << "\n  Total unpaid: " << config->currencySymbol << " "
              << std::fixed << std::setprecision(2) << unpaidTotal << "\n";
}
