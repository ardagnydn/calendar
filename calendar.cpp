#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "test1.h"

class EventBase {
public:
    virtual int getDay() const = 0;
    virtual int getMonth() const = 0;
    virtual int getYear() const = 0;
    virtual std::string getDescription() const = 0;
    virtual void print() const = 0;
    virtual ~EventBase() = default;
};

class GeneralEvent : public EventBase {
    int day, month, year;
    std::string description;

public:
    GeneralEvent(int d, int m, int y, const std::string &desc)
        : day(d), month(m), year(y), description(desc) {}

    int getDay() const override { return day; }
    int getMonth() const override { return month; }
    int getYear() const override { return year; }
    std::string getDescription() const override { return description; }

    void print() const override {
        std::cout << day << "/" << month << "/" << year << ": " << description << "\n";
    }
};

class Calendar {
    std::vector<EventBase *> events;

public:
    void addEvent(EventBase *e) {
        events.push_back(e);
    }

    const std::vector<EventBase *> &getEvents() const {
        return events;
    }

    void showAllEvents() const {
        for (const auto &e : events)
            e->print();
    }

    void showEventsOnDate(int d, int m, int y) const {
        for (const auto &e : events) {
            if (e->getDay() == d && e->getMonth() == m && e->getYear() == y)
                e->print();
        }
    }

    ~Calendar() {
        for (auto *e : events)
            delete e;
    }
};

TEST(CalendarTest, AddSingleEvent) {
    Calendar c;
    c.addEvent(new GeneralEvent(23, 6, 2025, "Final Exam"));
    const auto &events = c.getEvents();
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0]->getDay(), 23);
    ASSERT_EQ(events[0]->getMonth(), 6);
    ASSERT_EQ(events[0]->getYear(), 2025);
    ASSERT_EQ(events[0]->getDescription(), "Final Exam");
    return true;
}

TEST(CalendarTest, MultipleEventsCount) {
    Calendar c;
    c.addEvent(new GeneralEvent(1, 1, 2025, "New Year"));
    c.addEvent(new GeneralEvent(14, 2, 2025, "Valentine's Day"));
    c.addEvent(new GeneralEvent(25, 12, 2025, "Christmas"));
    ASSERT_EQ(c.getEvents().size(), 3);
    return true;
}

TEST(CalendarTest, EventMatchByDate) {
    Calendar c;
    c.addEvent(new GeneralEvent(10, 10, 2025, "Meeting"));
    c.addEvent(new GeneralEvent(10, 10, 2025, "Conference"));
    c.addEvent(new GeneralEvent(11, 10, 2025, "Workshop"));
    int count = 0;
    for (const auto &e : c.getEvents()) {
        if (e->getDay() == 10 && e->getMonth() == 10 && e->getYear() == 2025)
            count++;
    }
    ASSERT_EQ(count, 2);
    return true;
}

TEST(CalendarTest, EmptyCalendar) {
    Calendar c;
    ASSERT_EQ(c.getEvents().size(), 0);
    return true;
}

TEST(CalendarTest, AddEventSameDayDifferentYear) {
    Calendar c;
    c.addEvent(new GeneralEvent(1, 5, 2024, "Event A"));
    c.addEvent(new GeneralEvent(1, 5, 2025, "Event B"));
    ASSERT_EQ(c.getEvents().size(), 2);
    ASSERT_EQ(c.getEvents()[0]->getYear(), 2024);
    ASSERT_EQ(c.getEvents()[1]->getYear(), 2025);
    return true;
}

TEST(CalendarTest, AddLongDescriptionEvent) {
    Calendar c;
    std::string desc = "This is a very long event description meant to test storage and retrieval";
    c.addEvent(new GeneralEvent(2, 8, 2025, desc));
    ASSERT_EQ(c.getEvents().size(), 1);
    ASSERT_EQ(c.getEvents()[0]->getDescription(), desc);
    return true;
}

TEST(CalendarTest, AddEventsSameDate) {
    Calendar c;
    c.addEvent(new GeneralEvent(15, 7, 2025, "Morning Briefing"));
    c.addEvent(new GeneralEvent(15, 7, 2025, "Afternoon Workshop"));
    c.addEvent(new GeneralEvent(15, 7, 2025, "Evening Networking"));
    int count = 0;
    for (const auto &e : c.getEvents()) {
        if (e->getDay() == 15 && e->getMonth() == 7 && e->getYear() == 2025)
            count++;
    }
    ASSERT_EQ(count, 3);
    return true;
}

TEST(CalendarTest, CheckFirstAndLastEvent) {
    Calendar c;
    c.addEvent(new GeneralEvent(5, 3, 2025, "Start Project"));
    c.addEvent(new GeneralEvent(10, 6, 2025, "Mid Review"));
    c.addEvent(new GeneralEvent(20, 9, 2025, "Final Presentation"));
    ASSERT_EQ(c.getEvents().front()->getDescription(), "Start Project");
    ASSERT_EQ(c.getEvents().back()->getDescription(), "Final Presentation");
    return true;
}

int main() {
    RUN_TEST(CalendarTest, AddSingleEvent);
    RUN_TEST(CalendarTest, MultipleEventsCount);
    RUN_TEST(CalendarTest, EventMatchByDate);
    RUN_TEST(CalendarTest, EmptyCalendar);
    RUN_TEST(CalendarTest, AddEventSameDayDifferentYear);
    RUN_TEST(CalendarTest, AddLongDescriptionEvent);
    RUN_TEST(CalendarTest, AddEventsSameDate);
    RUN_TEST(CalendarTest, CheckFirstAndLastEvent);
    return 0;
}
