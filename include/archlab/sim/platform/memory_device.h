#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_PLATFORM_MEMORY_DEVICE_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_PLATFORM_MEMORY_DEVICE_H_

#include "archlab/sim/types.h"

#include <string_view>

namespace archlab::sim {

class MemoryDevice {
public:
    virtual ~MemoryDevice() = default;

    [[nodiscard]] virtual Data read64(Addr offset) = 0;
    virtual void write64(Addr offset, Data value) = 0;
    [[nodiscard]] virtual std::string_view device_name() const noexcept = 0;
};

}  // namespace archlab::sim

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_PLATFORM_MEMORY_DEVICE_H_
