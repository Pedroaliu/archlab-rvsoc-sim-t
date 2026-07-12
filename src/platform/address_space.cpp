#include "archlab/sim/platform/address_space.h"

#include <algorithm>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace archlab::sim {

void AddressSpace::map(Addr base, Addr size, MemoryDevice& device)
{
    if (size == 0) {
        throw std::invalid_argument("mapped region size must be non-zero");
    }
    if (base > std::numeric_limits<Addr>::max() - (size - 1)) {
        throw std::overflow_error("mapped region address overflow");
    }

    for (const Region& existing : regions_) {
        if (ranges_overlap(base, size, existing.base, existing.size)) {
            throw std::invalid_argument("mapped region overlaps an existing region");
        }
    }

    regions_.push_back(Region{base, size, &device});
    std::ranges::sort(regions_, {}, &Region::base);
}

Data AddressSpace::read64(Addr address)
{
    Region& region = resolve(address, sizeof(Data));
    return region.device->read64(address - region.base);
}

void AddressSpace::write64(Addr address, Data value)
{
    Region& region = resolve(address, sizeof(Data));
    region.device->write64(address - region.base, value);
}

std::size_t AddressSpace::region_count() const noexcept
{
    return regions_.size();
}

AddressSpace::Region& AddressSpace::resolve(Addr address, Addr access_size)
{
    for (Region& region : regions_) {
        if (address < region.base) {
            continue;
        }

        const Addr offset = address - region.base;
        if (offset <= region.size && access_size <= region.size - offset) {
            return region;
        }
    }

    std::ostringstream message;
    message << "unmapped or cross-region memory access at address 0x" << std::hex << address;
    throw std::out_of_range(message.str());
}

bool AddressSpace::ranges_overlap(Addr lhs_base,
                                  Addr lhs_size,
                                  Addr rhs_base,
                                  Addr rhs_size) noexcept
{
    const Addr lhs_end = lhs_base + lhs_size - 1;
    const Addr rhs_end = rhs_base + rhs_size - 1;
    return lhs_base <= rhs_end && rhs_base <= lhs_end;
}

}  // namespace archlab::sim
