#include "../include/InputValidator.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <limits>
#include <sstream>

// OOP Concept: Encapsulation - all cin reading is centralised here
// The program must NEVER crash: read as whole line, then convert

int InputValidator::readInt(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) {
            std::cout << "  [!] Input cannot be empty. Try again.\n";
            continue;
        }
        try {
            size_t pos;
            int val = std::stoi(line, &pos);
            if (pos != line.size()) {
                std::cout << "  [!] Please enter a whole number.\n";
                continue;
            }
            if (val < minVal || val > maxVal) {
                std::cout << "  [!] Value must be between " << minVal
                          << " and " << maxVal << ".\n";
                continue;
            }
            return val;
        } catch (...) {
            std::cout << "  [!] Invalid number. Please try again.\n";
        }
    }
}

double InputValidator::readDouble(const std::string& prompt, double minVal) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) {
            std::cout << "  [!] Input cannot be empty. Try again.\n";
            continue;
        }
        try {
            size_t pos;
            double val = std::stod(line, &pos);
            if (pos != line.size()) {
                std::cout << "  [!] Please enter a valid number.\n";
                continue;
            }
            if (val < minVal) {
                std::cout << "  [!] Value must be >= " << minVal << ".\n";
                continue;
            }
            return val;
        } catch (...) {
            std::cout << "  [!] Invalid number. Please try again.\n";
        }
    }
}

Date InputValidator::readDate(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        Date d = Date::parse(line);
        if (!d.isValid()) {
            std::cout << "  [!] Invalid date. Use DD/MM/YYYY (e.g. 25/12/2025).\n";
            continue;
        }
        return d;
    }
}

std::string InputValidator::readNonEmptyString(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) {
            std::cout << "  [!] This field cannot be empty. Try again.\n";
            continue;
        }
        return line;
    }
}

std::string InputValidator::readString(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}
