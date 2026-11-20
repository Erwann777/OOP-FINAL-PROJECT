#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include "Date.h"
#include <string>

// OOP Concept: Encapsulation - safe reading and checking of user input
// The ONLY place that calls std::cin (except Menu)
class InputValidator {
public:
    // Read a whole line then parse as integer; reprompts on failure
    static int         readInt(const std::string& prompt,
                                int minVal = INT_MIN, int maxVal = INT_MAX);

    // Read a whole line then parse as double; reprompts on failure
    static double      readDouble(const std::string& prompt,
                                   double minVal = 0.0);

    // Read a whole line and return a valid Date; reprompts on failure
    static Date        readDate(const std::string& prompt);

    // Read a non-empty string; reprompts on empty input
    static std::string readNonEmptyString(const std::string& prompt);

    // Read any string (can be empty, used for optional notes)
    static std::string readString(const std::string& prompt);
};

#endif // INPUTVALIDATOR_H
