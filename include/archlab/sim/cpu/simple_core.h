#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_CPU_SIMPLE_CORE_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_CPU_SIMPLE_CORE_H_

#include "archlab/sim/kernel/component.h"
#include "archlab/sim/platform/address_space.h"
#include "archlab/sim/stats/stats_manager.h"
#include "archlab/sim/types.h"

#include <cstdint>
#include <string>

namespace archlab::sim {

class SimpleCore final : public Component {
public:
    SimpleCore(std::string name,
               EventQueue& event_queue,
               AddressSpace& address_space,
               StatsManager& stats);

    void alu(std::uint64_t operation_count = 1) noexcept;
    [[nodiscard]] Data load64(Addr address);
    void store64(Addr address, Data value);
    void branch(bool taken) noexcept;

private:
    AddressSpace& address_space_;
    StatsManager& stats_;
};

}  // namespace archlab::sim

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_CPU_SIMPLE_CORE_H_
