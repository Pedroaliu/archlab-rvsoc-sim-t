#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_STATS_STATS_MANAGER_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_STATS_STATS_MANAGER_H_

#include "archlab/sim/types.h"

#include <cstdint>
#include <iosfwd>

namespace archlab::sim {

class StatsManager {
public:
    struct Counters {
        std::uint64_t instructions{};
        std::uint64_t alu_operations{};
        std::uint64_t loads{};
        std::uint64_t stores{};
        std::uint64_t branches{};
        std::uint64_t branches_taken{};
    };

    struct Snapshot {
        Counters total;
        Counters roi;
        Tick roi_cycles{};
        bool roi_active{};
    };

    void reset(Tick now) noexcept;
    void begin_roi(Tick now);
    void end_roi(Tick now);

    void record_alu(std::uint64_t count = 1) noexcept;
    void record_load() noexcept;
    void record_store() noexcept;
    void record_branch(bool taken) noexcept;

    [[nodiscard]] bool roi_active() const noexcept;
    [[nodiscard]] Snapshot snapshot(Tick now) const noexcept;
    void dump(std::ostream& output, Tick now) const;

private:
    static void add(Counters& destination, const Counters& delta) noexcept;
    void record(const Counters& delta) noexcept;
    [[nodiscard]] Tick current_roi_cycles(Tick now) const noexcept;

    Counters total_;
    Counters roi_;
    Tick roi_start_{};
    Tick accumulated_roi_cycles_{};
    bool roi_active_{};
};

}  // namespace archlab::sim

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_STATS_STATS_MANAGER_H_
