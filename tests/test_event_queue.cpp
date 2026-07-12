#include "test_framework.h"

#include "archlab/sim/kernel/event_queue.h"

#include <stdexcept>
#include <string>
#include <vector>

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
