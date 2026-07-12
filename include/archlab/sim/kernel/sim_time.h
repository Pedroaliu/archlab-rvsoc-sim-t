//
// Created by pedroa on 2026-07-12.
//

#ifndef ARCHLAB_RVSOC_SIM_SIM_TIME_H
#define ARCHLAB_RVSOC_SIM_SIM_TIME_H

#include <compare>
#include <cstdint>

namespace archlab::sim {

using Tick = std::uint64_t;
using Delta = std::uint32_t;
using Sequence = std::uint64_t;

enum class EventPhase : std::uint8_t {
    Input = 0,
    Evaluate = 1,
    Commit = 2,
    Notify = 3,
};

struct EventStamp {
    Tick tick{};
    EventPhase phase {EventPhase::Input};
    Delta delta {};
    Sequence sequence {};

    auto operator<=>(const EventStamp& ) const = default;
};

}  // namespace archlab::sim

#endif // ARCHLAB_RVSOC_SIM_SIM_TIME_H
