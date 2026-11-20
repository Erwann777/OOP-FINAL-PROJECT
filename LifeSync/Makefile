CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Source files
SRCS = main.cpp \
       src/Date.cpp \
       src/Task.cpp \
       src/Assignment.cpp \
       src/Appointment.cpp \
       src/Habit.cpp \
       src/Bill.cpp \
       src/Expense.cpp \
       src/Planner.cpp \
       src/InputValidator.cpp \
       src/FileManager.cpp \
       src/TaskFactory.cpp \
       src/Menu.cpp

TARGET = lifesync

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	del /Q $(TARGET).exe 2>nul || rm -f $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
