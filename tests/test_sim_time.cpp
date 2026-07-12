//
// Created by pedroa on 2026-07-12.
//


#include "archlab/sim/kernel/sim_time.h"
#include "test_framework.h"


using archlab::sim::EventPhase;
using archlab::sim::EventStamp;

void test_event_stamp_ordering() {
    const EventStamp evaluate {
        .tick = 1000,
        .phase = EventPhase::Evaluate,
        .delta = 9,
        .sequence = 100,
    };

    const EventStamp commit {
        .tick = 1000,
        .phase = EventPhase::Commit,
        .delta = 0,
        .sequence = 5,
    };

    ARCHLAB_CHECK(evaluate < commit);

    const EventStamp first {
        .tick = 1000,
        .phase = EventPhase::Evaluate,
        .delta = 2,
        .sequence = 20,
    };

    const EventStamp second {
        .tick = 1000,
        .phase = EventPhase::Evaluate,
        .delta = 3,
        .sequence = 10,
    };

    ARCHLAB_CHECK(first < second);
}