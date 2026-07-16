#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_KERNEL_CLOCK_DOMAIN_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_KERNEL_CLOCK_DOMAIN_H_

#include "archlab/sim/kernel/sim_time.h"

#include <limits>
#include <stdexcept>

namespace archlab::sim {

class ClockDomain {
public:
    explicit ClockDomain(Tick period, Tick offset = 0) : period_(period), offset_(offset)
    {
        if (period_ == 0) {
            throw std::invalid_argument("clock period must be greater than zero");
        }
    }

    [[nodiscard]] Tick period() const noexcept
    {
        return period_;
    }

    [[nodiscard]] Tick offset() const noexcept
    {
        return offset_;
    }

    [[nodiscard]] bool is_edge(Tick tick) const noexcept
    {
        return tick >= offset_ && ((tick - offset_) % period_) == 0;
    }

    [[nodiscard]] Tick edge_at_or_after(Tick tick) const
    {
        if (tick <= offset_) {
            return offset_;
        }

        const Tick remainder = (tick - offset_) % period_;
        if (remainder == 0) {
            return tick;
        }

        return checked_add(tick, period_ - remainder);
    }

    [[nodiscard]] Tick next_edge_after(Tick tick) const
    {
        if (tick < offset_) {
            return offset_;
        }

        const Tick edge = edge_at_or_after(tick);
        if (edge != tick) {
            return edge;
        }

        return checked_add(tick, period_);
    }

private:
    [[nodiscard]] static Tick checked_add(Tick lhs, Tick rhs)
    {
        if (rhs > std::numeric_limits<Tick>::max() - lhs) {
            throw std::overflow_error("clock edge time overflow");
        }
        return lhs + rhs;
    }

    Tick period_;
    Tick offset_;
};

}  // namespace archlab::sim

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_KERNEL_CLOCK_DOMAIN_H_
