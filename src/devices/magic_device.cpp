#include "archlab/sim/devices/magic_device.h"

#include <iostream>
#include <stdexcept>
#include <utility>

namespace archlab::sim {

MagicDevice::MagicDevice(std::string name, EventQueue& event_queue, StatsManager& stats)
    : Component(std::move(name), event_queue), stats_(stats)
{
}

Data MagicDevice::read64(Addr offset)
{
    if (offset != 0) {
        throw std::out_of_range("magic device only implements offset zero");
    }
    return last_command_;
}

void MagicDevice::write64(Addr offset, Data value)
{
    if (offset != 0) {
        throw std::out_of_range("magic device only implements offset zero");
    }

    last_command_ = value;
    const Tick now = event_queue().now();

    switch (value) {
    case kResetStats:
        stats_.reset(now);
        break;
    case kBeginRoi:
        stats_.begin_roi(now);
        break;
    case kEndRoi:
        stats_.end_roi(now);
        break;
    case kDumpStats:
        stats_.dump(std::cout, now);
        break;
    default:
        throw std::invalid_argument("unknown magic device command");
    }
}

std::string_view MagicDevice::device_name() const noexcept
{
    return name();
}

}  // namespace archlab::sim
