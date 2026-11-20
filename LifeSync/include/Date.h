#ifndef DATE_H
#define DATE_H

#include <string>
#include <ostream>

// Value class for a calendar date with validation and comparison
// OOP Concept: Encapsulation - private data with validated setters
class Date {
private:
    int day;
    int month;
    int year;

public:
    Date();
    Date(int day, int month, int year);

    // Getters (Encapsulation)
    int getDay()   const { return day; }
    int getMonth() const { return month; }
    int getYear()  const { return year; }

    bool isValid() const;

    // Returns how many days until (or since) this date from today
    int daysUntil() const;

    // Returns today's date
    static Date today();

    // Parses a date from "DD/MM/YYYY" string; returns invalid Date on failure
    static Date parse(const std::string& s);

    // Formats date as "DD/MM/YYYY"
    std::string toString() const;

    // OOP Concept: Operator overloading
    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;

    // Stream output operator
    friend std::ostream& operator<<(std::ostream& os, const Date& d);
};

#endif // DATE_H
