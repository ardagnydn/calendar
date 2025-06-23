#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "test1.h"

class Event {
    int day, month, year;
    std::string description;

public:
    Event(int d, int m, int y, const std::string &desc)
        : day(d), month(m), year(y), description(desc) {}

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    std::string getDescription() const { return description; }

    void print() const {
        std::cout << day << "/" << month << "/" << year << ": " << description << "\n";
    }
};

class Calendar {
    std::vector<Event> events;

public:
    void addEvent(const Event &e) {
        events.push_back(e);
    }

    const std::vector<Event> &getEvents() const {
        return events;
    }

    void showAllEvents() const {
        for (const auto &e : events)
            e.print();
    }

    void showEventsOnDate(int d, int m, int y) const {
        for (const auto &e : events) {
            if (e.getDay() == d && e.getMonth() == m && e.getYear() == y)
                e.print();
        }
    }
};

TEST(CalendarTest, AddSingleEvent) {
    Calendar c;
    c.addEvent(Event(23, 6, 2025, "Final Exam"));
    const auto &events = c.getEvents();
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].getDay(), 23);
    ASSERT_EQ(events[0].getMonth(), 6);
    ASSERT_EQ(events[0].getYear(), 2025);
    ASSERT_EQ(events[0].getDescription(), "Final Exam");
    return true;
}

int main() {
    RUN_TEST(CalendarTest, AddSingleEvent);
    return 0;
}
