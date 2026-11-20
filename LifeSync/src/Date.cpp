#include "../include/Date.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <ctime>

// ---- Constructors ----
Date::Date() : day(1), month(1), year(2000) {}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

// ---- Validation ----
bool Date::isValid() const {
    if (year < 1900 || year > 9999) return false;
    if (month < 1 || month > 12)    return false;
    if (day < 1)                    return false;

    // Days per month (non-leap year)
    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

    // Leap year check
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (leap) daysInMonth[2] = 29;

    return day <= daysInMonth[month];
}

// ---- Static helpers ----
Date Date::today() {
    std::time_t t = std::time(nullptr);
    std::tm* tm   = std::localtime(&t);
    return Date(tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

Date Date::parse(const std::string& s) {
    // Expected format: DD/MM/YYYY
    if (s.size() != 10 || s[2] != '/' || s[5] != '/') {
        return Date(0, 0, 0); // invalid sentinel
    }
    try {
        int d = std::stoi(s.substr(0, 2));
        int m = std::stoi(s.substr(3, 2));
        int y = std::stoi(s.substr(6, 4));
        return Date(d, m, y);
    } catch (...) {
        return Date(0, 0, 0); // invalid sentinel
    }
}

// ---- Conversion ----
std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << day  << "/"
        << std::setw(2) << month << "/"
        << std::setw(4) << year;
    return oss.str();
}

// ---- daysUntil: positive = future, negative = past ----
int Date::daysUntil() const {
    Date td = today();

    // Convert both dates to tm and use difftime
    std::tm thisDate{};
    thisDate.tm_mday = day;
    thisDate.tm_mon  = month - 1;
    thisDate.tm_year = year - 1900;

    std::tm todayDate{};
    todayDate.tm_mday = td.day;
    todayDate.tm_mon  = td.month - 1;
    todayDate.tm_year = td.year - 1900;

    std::time_t t1 = std::mktime(&thisDate);
    std::time_t t2 = std::mktime(&todayDate);

    // difftime returns seconds; 86400 seconds per day
    double diff = std::difftime(t1, t2);
    return static_cast<int>(diff / 86400.0);
}

// ---- Operator overloading (OOP Concept: Overloading) ----
bool Date::operator<(const Date& o) const {
    if (year  != o.year)  return year  < o.year;
    if (month != o.month) return month < o.month;
    return day < o.day;
}

bool Date::operator==(const Date& o) const {
    return day == o.day && month == o.month && year == o.year;
}

bool Date::operator<=(const Date& o) const {
    return (*this < o) || (*this == o);
}

bool Date::operator>(const Date& o) const {
    return !(*this <= o);
}

std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << d.toString();
    return os;
}
