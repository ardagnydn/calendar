#include <iostream>
#include <vector>
#include <string>
#include "test1.h"

class GeneralEvent;

class CalendarAction {
public:
    virtual void perform(GeneralEvent &event) = 0;
    virtual std::string description() const = 0;
    virtual ~CalendarAction() = default;
};

class GeneralEvent {
    int day, month, year;
    std::string event_description;
    std::string event_type;
    bool is_important;
    std::vector<CalendarAction *> scheduled_actions;

public:
    GeneralEvent(int d, int m, int y, const std::string &desc,
                 const std::string &type, bool important,
                 const std::vector<CalendarAction *> &actions)
        : day(d), month(m), year(y), event_description(desc),
          event_type(type), is_important(important),
          scheduled_actions(actions) {}

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    std::string getEventDescription() const { return event_description; }
    std::string getEventType() const { return event_type; }
    bool isImportant() const { return is_important; }

    void setEventDescription(const std::string &desc) { event_description = desc; }
    void setEventType(const std::string &type) { event_type = type; }
    void setImportant(bool important) { is_important = important; }

    void triggerActions() {
        for (auto *action : scheduled_actions) {
            action->perform(*this);
        }
    }

    GeneralEvent(const GeneralEvent &) = delete;
    GeneralEvent &operator=(const GeneralEvent &) = delete;

    ~GeneralEvent() {
        for (auto *action : scheduled_actions) {
            delete action;
        }
        scheduled_actions.clear();
    }
};

class SendNotification : public CalendarAction {
    std::string notification_message;
public:
    SendNotification(const std::string &msg) : notification_message(msg) {}
    void perform(GeneralEvent &) override {}
    std::string description() const override { return ""; }
};

class MarkAsImportant : public CalendarAction {
public:
    void perform(GeneralEvent &event) override { event.setImportant(true); }
    std::string description() const override { return ""; }
};

class LogEventDetails : public CalendarAction {
public:
    void perform(GeneralEvent &) override {}
    std::string description() const override { return ""; }
};

class ConditionalAction : public CalendarAction {
    CalendarAction *inner_action;
    std::string condition_type;
    int condition_value;
    bool triggered_once = false;

public:
    ConditionalAction(CalendarAction *action, const std::string &cond_type, int cond_val = 0)
        : inner_action(action), condition_type(cond_type), condition_value(cond_val) {}

    void perform(GeneralEvent &e) override {
        if (triggered_once) return;
        if ((condition_type == "OnDay" && e.getDay() == condition_value) ||
            (condition_type == "IfImportant" && e.isImportant())) {
            inner_action->perform(e);
            triggered_once = true;
        }
    }

    std::string description() const override { return ""; }

    ~ConditionalAction() override { delete inner_action; }
};

class Calendar {
    std::vector<GeneralEvent *> events;

public:
    void addEvent(GeneralEvent *e) { events.push_back(e); }
    const std::vector<GeneralEvent *> &getEvents() const { return events; }

    void checkAndTriggerEventActions(int current_day) {
        for (auto* event : events) {
            if (event->getDay() == current_day) {
                event->triggerActions();
            }
        }
    }

    ~Calendar() {
        for (auto *e : events) delete e;
        events.clear();
    }
};

TEST(CalendarTest, AddSingleEvent) {
    Calendar c;
    std::vector<CalendarAction*> actions = { new LogEventDetails() };
    c.addEvent(new GeneralEvent(23, 6, 2025, "Final Exam", "Exam", false, actions));
    const auto &events = c.getEvents();
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0]->getDay(), 23);
    ASSERT_EQ(events[0]->getMonth(), 6);
    ASSERT_EQ(events[0]->getYear(), 2025);
    ASSERT_EQ(events[0]->getEventDescription(), "Final Exam");
    return true;
}

TEST(CalendarTest, MultipleEventsCount) {
    Calendar c;
    c.addEvent(new GeneralEvent(1, 1, 2025, "New Year", "Holiday", false, {}));
    c.addEvent(new GeneralEvent(14, 2, 2025, "Valentine's Day", "Celebration", false, {}));
    c.addEvent(new GeneralEvent(25, 12, 2025, "Christmas", "Holiday", false, {}));
    ASSERT_EQ(c.getEvents().size(), 3);
    return true;
}

TEST(CalendarTest, EventMatchByDate) {
    Calendar c;
    c.addEvent(new GeneralEvent(10, 10, 2025, "Meeting", "Work", false, {}));
    c.addEvent(new GeneralEvent(10, 10, 2025, "Conference", "Work", false, {}));
    c.addEvent(new GeneralEvent(11, 10, 2025, "Workshop", "Education", false, {}));
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
    c.addEvent(new GeneralEvent(1, 5, 2024, "Event A", "Personal", false, {}));
    c.addEvent(new GeneralEvent(1, 5, 2025, "Event B", "Personal", false, {}));
    ASSERT_EQ(c.getEvents().size(), 2);
    ASSERT_EQ(c.getEvents()[0]->getYear(), 2024);
    ASSERT_EQ(c.getEvents()[1]->getYear(), 2025);
    return true;
}

TEST(CalendarTest, AddLongDescriptionEvent) {
    Calendar c;
    std::string desc = "This is a very long event description meant to test storage and retrieval";
    c.addEvent(new GeneralEvent(2, 8, 2025, desc, "Test", false, {}));
    ASSERT_EQ(c.getEvents().size(), 1);
    ASSERT_EQ(c.getEvents()[0]->getEventDescription(), desc);
    return true;
}

TEST(CalendarTest, AddEventsSameDate) {
    Calendar c;
    c.addEvent(new GeneralEvent(15, 7, 2025, "Morning Briefing", "Work", false, {}));
    c.addEvent(new GeneralEvent(15, 7, 2025, "Afternoon Workshop", "Work", false, {}));
    c.addEvent(new GeneralEvent(15, 7, 2025, "Evening Networking", "Social", false, {}));
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
    c.addEvent(new GeneralEvent(5, 3, 2025, "Start Project", "Work", false, {}));
    c.addEvent(new GeneralEvent(10, 6, 2025, "Mid Review", "Work", false, {}));
    c.addEvent(new GeneralEvent(20, 9, 2025, "Final Presentation", "Work", false, {}));
    ASSERT_EQ(c.getEvents().front()->getEventDescription(), "Start Project");
    ASSERT_EQ(c.getEvents().back()->getEventDescription(), "Final Presentation");
    return true;
}

TEST(CalendarTest, ActionTriggering) {
    Calendar c;
    std::vector<CalendarAction*> actions1 = { new SendNotification("Meeting starting soon!"), new LogEventDetails() };
    std::vector<CalendarAction*> actions2 = { new MarkAsImportant(), new ConditionalAction(new SendNotification("Important Task Reminder!"), "IfImportant") };

    c.addEvent(new GeneralEvent(25, 6, 2025, "Daily Standup", "Work", false, actions1));
    c.addEvent(new GeneralEvent(26, 6, 2025, "Project Deadline", "Work", true, actions2));

    c.checkAndTriggerEventActions(25);
    c.checkAndTriggerEventActions(26);

    const auto& events = c.getEvents();
    bool projectDeadlineFound = false;
    for (const auto& e : events) {
        if (e->getEventDescription() == "Project Deadline") {
            ASSERT_TRUE(e->isImportant());
            projectDeadlineFound = true;
            break;
        }
    }
    ASSERT_TRUE(projectDeadlineFound);

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
    RUN_TEST(CalendarTest, ActionTriggering);
    return 0;
}
