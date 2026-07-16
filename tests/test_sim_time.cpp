#include "test_framework.h"

#include "archlab/sim/kernel/clock_domain.h"
#include "archlab/sim/kernel/sim_time.h"

#include <limits>
#include <stdexcept>

using archlab::sim::ClockDomain;
using archlab::sim::EventPhase;
using archlab::sim::EventStamp;
using archlab::sim::Tick;

ARCHLAB_TEST("EventStamp orders by tick, phase, delta, then sequence")
{
    const EventStamp earlier_tick{
        .tick = 9,
        .phase = EventPhase::Notify,
        .delta = std::numeric_limits<archlab::sim::Delta>::max(),
        .sequence = std::numeric_limits<archlab::sim::Sequence>::max(),
    };
    const EventStamp later_tick{
        .tick = 10,
        .phase = EventPhase::Input,
        .delta = 0,
        .sequence = 0,
    };
    ARCHLAB_CHECK(earlier_tick < later_tick);

    const EventStamp earlier_phase{
        .tick = 1000,
        .phase = EventPhase::Evaluate,
        .delta = 9,
        .sequence = 100,
    };
    const EventStamp later_phase{
        .tick = 1000,
        .phase = EventPhase::Commit,
        .delta = 0,
        .sequence = 5,
    };
    ARCHLAB_CHECK(earlier_phase < later_phase);

    const EventStamp earlier_delta{
        .tick = 1000,
        .phase = EventPhase::Evaluate,
        .delta = 2,
        .sequence = 20,
    };
    const EventStamp later_delta{
        .tick = 1000,
        .phase = EventPhase::Evaluate,
        .delta = 3,
        .sequence = 10,
    };
    ARCHLAB_CHECK(earlier_delta < later_delta);

    const EventStamp earlier_sequence{
        .tick = 1000,
        .phase = EventPhase::Evaluate,
        .delta = 3,
        .sequence = 10,
    };
    const EventStamp later_sequence{
        .tick = 1000,
        .phase = EventPhase::Evaluate,
        .delta = 3,
        .sequence = 11,
    };
    ARCHLAB_CHECK(earlier_sequence < later_sequence);
}

ARCHLAB_TEST("ClockDomain rejects a zero period")
{
    ARCHLAB_CHECK_THROWS_AS(ClockDomain(0), std::invalid_argument);
}

ARCHLAB_TEST("ClockDomain distinguishes inclusive and strict edge queries")
{
    const ClockDomain clock{10, 3};

    ARCHLAB_CHECK_EQ(clock.period(), Tick{10});
    ARCHLAB_CHECK_EQ(clock.offset(), Tick{3});

    ARCHLAB_CHECK(!clock.is_edge(2));
    ARCHLAB_CHECK(clock.is_edge(3));
    ARCHLAB_CHECK(!clock.is_edge(12));
    ARCHLAB_CHECK(clock.is_edge(13));

    ARCHLAB_CHECK_EQ(clock.edge_at_or_after(0), Tick{3});
    ARCHLAB_CHECK_EQ(clock.edge_at_or_after(3), Tick{3});
    ARCHLAB_CHECK_EQ(clock.edge_at_or_after(4), Tick{13});
    ARCHLAB_CHECK_EQ(clock.edge_at_or_after(13), Tick{13});

    ARCHLAB_CHECK_EQ(clock.next_edge_after(0), Tick{3});
    ARCHLAB_CHECK_EQ(clock.next_edge_after(2), Tick{3});
    ARCHLAB_CHECK_EQ(clock.next_edge_after(3), Tick{13});
    ARCHLAB_CHECK_EQ(clock.next_edge_after(4), Tick{13});
    ARCHLAB_CHECK_EQ(clock.next_edge_after(13), Tick{23});
}

ARCHLAB_TEST("ClockDomain reports unrepresentable future edges")
{
    const Tick max_tick = std::numeric_limits<Tick>::max();

    const ClockDomain no_representable_edge{4, 1};
    ARCHLAB_CHECK_THROWS_AS(no_representable_edge.edge_at_or_after(max_tick),
                            std::overflow_error);

    const ClockDomain edge_at_max{4, max_tick};
    ARCHLAB_CHECK_EQ(edge_at_max.edge_at_or_after(max_tick), max_tick);
    ARCHLAB_CHECK_THROWS_AS(edge_at_max.next_edge_after(max_tick), std::overflow_error);
}
