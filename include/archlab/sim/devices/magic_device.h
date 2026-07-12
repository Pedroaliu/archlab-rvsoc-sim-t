#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_DEVICES_MAGIC_DEVICE_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_DEVICES_MAGIC_DEVICE_H_

#include "archlab/sim/kernel/component.h"
#include "archlab/sim/platform/memory_device.h"
#include "archlab/sim/stats/stats_manager.h"
#include "archlab/sim/types.h"

#include <string>
#include <string_view>

namespace archlab::sim {

class MagicDevice final : public Component, public MemoryDevice {
public:
    static constexpr Data kResetStats = 0x1;
    static constexpr Data kBeginRoi = 0x2;
    static constexpr Data kEndRoi = 0x3;
    static constexpr Data kDumpStats = 0x4;

    MagicDevice(std::string name, EventQueue& event_queue, StatsManager& stats);

    [[nodiscard]] Data read64(Addr offset) override;
    void write64(Addr offset, Data value) override;
    [[nodiscard]] std::string_view device_name() const noexcept override;

private:
    StatsManager& stats_;
    Data last_command_{};
};

}  // namespace archlab::sim

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_DEVICES_MAGIC_DEVICE_H_
