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

TEST(CalendarTest, MultipleEventsCount) {
    Calendar c;
    c.addEvent(Event(1, 1, 2025, "New Year"));
    c.addEvent(Event(14, 2, 2025, "Valentine's Day"));
    c.addEvent(Event(25, 12, 2025, "Christmas"));
    ASSERT_EQ(c.getEvents().size(), 3);
    return true;
}

TEST(CalendarTest, EventMatchByDate) {
    Calendar c;
    c.addEvent(Event(10, 10, 2025, "Meeting"));
    c.addEvent(Event(10, 10, 2025, "Conference"));
    c.addEvent(Event(11, 10, 2025, "Workshop"));

    int count = 0;
    for (const auto &e : c.getEvents()) {
        if (e.getDay() == 10 && e.getMonth() == 10 && e.getYear() == 2025)
            count++;
    }

    ASSERT_EQ(count, 2);
    return true;
}

int main() {
    RUN_TEST(CalendarTest, AddSingleEvent);
    RUN_TEST(CalendarTest, MultipleEventsCount);
    RUN_TEST(CalendarTest, EventMatchByDate);
    return 0;
}
