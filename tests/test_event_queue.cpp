#include "test_framework.h"

#include "archlab/sim/kernel/event_queue.h"
#include "archlab/sim/kernel/sim_time.h"

#include <stdexcept>
#include <string>
#include <vector>

using archlab::sim::EventPhase;
using archlab::sim::EventQueue;

ARCHLAB_TEST("EventQueue orders events by tick and insertion sequence")
{
    EventQueue queue;
    std::vector<std::string> order;

    static_cast<void>(queue.schedule_abs(10, "first at ten", [&]() { order.emplace_back("a"); }));
    static_cast<void>(queue.schedule_abs(5, "at five", [&]() { order.emplace_back("b"); }));
    static_cast<void>(queue.schedule_abs(10, "second at ten", [&]() { order.emplace_back("c"); }));

    queue.run_until(10);

    ARCHLAB_CHECK_EQ(order.size(), 3U);
    ARCHLAB_CHECK_EQ(order.at(0), "b");
    ARCHLAB_CHECK_EQ(order.at(1), "a");
    ARCHLAB_CHECK_EQ(order.at(2), "c");
    ARCHLAB_CHECK_EQ(queue.now(), 10U);
}

ARCHLAB_TEST("EventQueue supports cancellation")
{
    EventQueue queue;
    bool executed = false;

    const EventQueue::EventId event_id =
        queue.schedule_abs(20, "cancelled", [&]() { executed = true; });

    ARCHLAB_CHECK(queue.cancel(event_id));
    ARCHLAB_CHECK(!queue.cancel(event_id));
    queue.run_until(30);

    ARCHLAB_CHECK(!executed);
    ARCHLAB_CHECK(queue.empty());
}

ARCHLAB_TEST("EventQueue rejects events in the simulated past")
{
    EventQueue queue;
    queue.run_until(10);

    ARCHLAB_CHECK_THROWS_AS(queue.schedule_abs(9, "past", []() {}), std::invalid_argument);
}

ARCHLAB_TEST("EventQueue orders same-tick events by phase")
{
    EventQueue queue;
    std::vector<std::string> order;

    static_cast<void>(queue.schedule_abs(
        10, EventPhase::Commit, 0, "commit", [&]() { order.emplace_back("commit"); }));
    static_cast<void>(queue.schedule_abs(
        10, EventPhase::Input, 0, "input", [&]() { order.emplace_back("input"); }));
    static_cast<void>(queue.schedule_abs(
        10, EventPhase::Notify, 0, "notify", [&]() { order.emplace_back("notify"); }));
    static_cast<void>(queue.schedule_abs(10, EventPhase::Evaluate, 0, "evaluate", [&]() {
        order.emplace_back("evaluate");
    }));

    queue.run_until(10);

    ARCHLAB_CHECK_EQ(order.size(), 4U);
    ARCHLAB_CHECK_EQ(order.at(0), "input");
    ARCHLAB_CHECK_EQ(order.at(1), "evaluate");
    ARCHLAB_CHECK_EQ(order.at(2), "commit");
    ARCHLAB_CHECK_EQ(order.at(3), "notify");
}

ARCHLAB_TEST("EventQueue orders same-phase events by delta and insertion sequence")
{
    EventQueue queue;
    std::vector<std::string> order;

    static_cast<void>(queue.schedule_abs(10, EventPhase::Evaluate, 1, "delta one first", [&]() {
        order.emplace_back("delta-one-first");
    }));
    static_cast<void>(queue.schedule_abs(10, EventPhase::Evaluate, 0, "delta zero", [&]() {
        order.emplace_back("delta-zero");
    }));
    static_cast<void>(queue.schedule_abs(10, EventPhase::Evaluate, 1, "delta one second", [&]() {
        order.emplace_back("delta-one-second");
    }));

    queue.run();

    ARCHLAB_CHECK_EQ(order.size(), 3U);
    ARCHLAB_CHECK_EQ(order.at(0), "delta-zero");
    ARCHLAB_CHECK_EQ(order.at(1), "delta-one-first");
    ARCHLAB_CHECK_EQ(order.at(2), "delta-one-second");
}

ARCHLAB_TEST("EventQueue rejects phase or delta rollback at the current tick")
{
    EventQueue queue;

    static_cast<void>(queue.schedule_abs(10, EventPhase::Commit, 2, "commit", [&]() {
        ARCHLAB_CHECK_THROWS_AS(
            queue.schedule_abs(10, EventPhase::Evaluate, 9, "earlier phase", []() {}),
            std::invalid_argument);
        ARCHLAB_CHECK_THROWS_AS(
            queue.schedule_abs(10, EventPhase::Commit, 1, "earlier delta", []() {}),
            std::invalid_argument);
    }));

    queue.run();
}

ARCHLAB_TEST("EventQueue allows same-phase same-delta scheduling by sequence")
{
    EventQueue queue;
    std::vector<std::string> order;

    static_cast<void>(queue.schedule_abs(10, EventPhase::Evaluate, 0, "first", [&]() {
        order.emplace_back("first");
        static_cast<void>(queue.schedule_abs(10, EventPhase::Evaluate, 0, "second", [&]() {
            order.emplace_back("second");
        }));
    }));

    queue.run();

    ARCHLAB_CHECK_EQ(order.size(), 2U);
    ARCHLAB_CHECK_EQ(order.at(0), "first");
    ARCHLAB_CHECK_EQ(order.at(1), "second");
}
