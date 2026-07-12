#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_DEVICES_SIMPLE_RAM_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_DEVICES_SIMPLE_RAM_H_

#include "archlab/sim/platform/memory_device.h"
#include "archlab/sim/types.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace archlab::sim {

class SimpleRam final : public MemoryDevice {
public:
    SimpleRam(std::string name, Addr size);

    [[nodiscard]] Data read64(Addr offset) override;
    void write64(Addr offset, Data value) override;
    [[nodiscard]] std::string_view device_name() const noexcept override;

    [[nodiscard]] Addr size() const noexcept;

private:
    void check_access(Addr offset, Addr access_size) const;

    std::string name_;
    std::vector<std::uint8_t> bytes_;
};

}  // namespace archlab::sim

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_DEVICES_SIMPLE_RAM_H_
