#include "archlab/sim/stats/stats_manager.h"

#include <ostream>
#include <stdexcept>

namespace archlab::sim {

void StatsManager::reset(Tick now) noexcept
{
    total_ = {};
    roi_ = {};
    roi_start_ = now;
    accumulated_roi_cycles_ = 0;
    roi_active_ = false;
}

void StatsManager::begin_roi(Tick now)
{
    if (roi_active_) {
        throw std::logic_error("ROI is already active");
    }

    roi_start_ = now;
    roi_active_ = true;
}

void StatsManager::end_roi(Tick now)
{
    if (!roi_active_) {
        throw std::logic_error("ROI is not active");
    }
    if (now < roi_start_) {
        throw std::logic_error("ROI end time is earlier than its start time");
    }

    accumulated_roi_cycles_ += now - roi_start_;
    roi_active_ = false;
}

void StatsManager::record_alu(std::uint64_t count) noexcept
{
    record(Counters{count, count, 0, 0, 0, 0});
}

void StatsManager::record_load() noexcept
{
    record(Counters{1, 0, 1, 0, 0, 0});
}

void StatsManager::record_store() noexcept
{
    record(Counters{1, 0, 0, 1, 0, 0});
}

void StatsManager::record_branch(bool taken) noexcept
{
    record(Counters{1, 0, 0, 0, 1, taken ? 1U : 0U});
}

bool StatsManager::roi_active() const noexcept
{
    return roi_active_;
}

StatsManager::Snapshot StatsManager::snapshot(Tick now) const noexcept
{
    return Snapshot{total_, roi_, current_roi_cycles(now), roi_active_};
}

void StatsManager::dump(std::ostream& output, Tick now) const
{
    const Snapshot current = snapshot(now);

    output << "[stats] tick=" << now << '\n'
           << "  total.instructions=" << current.total.instructions << '\n'
           << "  total.alu_operations=" << current.total.alu_operations << '\n'
           << "  total.loads=" << current.total.loads << '\n'
           << "  total.stores=" << current.total.stores << '\n'
           << "  total.branches=" << current.total.branches << '\n'
           << "  roi.active=" << current.roi_active << '\n'
           << "  roi.cycles=" << current.roi_cycles << '\n'
           << "  roi.instructions=" << current.roi.instructions << '\n'
           << "  roi.loads=" << current.roi.loads << '\n'
           << "  roi.stores=" << current.roi.stores << '\n'
           << "  roi.branches=" << current.roi.branches << '\n';
}

void StatsManager::add(Counters& destination, const Counters& delta) noexcept
{
    destination.instructions += delta.instructions;
    destination.alu_operations += delta.alu_operations;
    destination.loads += delta.loads;
    destination.stores += delta.stores;
    destination.branches += delta.branches;
    destination.branches_taken += delta.branches_taken;
}

void StatsManager::record(const Counters& delta) noexcept
{
    add(total_, delta);
    if (roi_active_) {
        add(roi_, delta);
    }
}

Tick StatsManager::current_roi_cycles(Tick now) const noexcept
{
    if (!roi_active_ || now < roi_start_) {
        return accumulated_roi_cycles_;
    }
    return accumulated_roi_cycles_ + (now - roi_start_);
}

}  // namespace archlab::sim
