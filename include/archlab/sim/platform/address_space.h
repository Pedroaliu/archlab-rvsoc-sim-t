#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_PLATFORM_ADDRESS_SPACE_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_PLATFORM_ADDRESS_SPACE_H_

#include "archlab/sim/platform/memory_device.h"
#include "archlab/sim/types.h"

#include <cstddef>
#include <vector>

namespace archlab::sim {

class AddressSpace {
public:
    AddressSpace() = default;

    void map(Addr base, Addr size, MemoryDevice& device);

    [[nodiscard]] Data read64(Addr address);
    void write64(Addr address, Data value);

    [[nodiscard]] std::size_t region_count() const noexcept;

private:
    struct Region {
        Addr base{};
        Addr size{};
        MemoryDevice* device{};
    };

    [[nodiscard]] Region& resolve(Addr address, Addr access_size);
    [[nodiscard]] static bool ranges_overlap(Addr lhs_base,
                                             Addr lhs_size,
                                             Addr rhs_base,
                                             Addr rhs_size) noexcept;

    std::vector<Region> regions_;
};

}  // namespace archlab::sim

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_PLATFORM_ADDRESS_SPACE_H_
