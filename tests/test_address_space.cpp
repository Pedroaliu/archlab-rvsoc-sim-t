#include "test_framework.h"

#include "archlab/sim/devices/simple_ram.h"
#include "archlab/sim/platform/address_space.h"

#include <stdexcept>

using archlab::sim::AddressSpace;
using archlab::sim::SimpleRam;

ARCHLAB_TEST("AddressSpace routes little-endian 64-bit RAM accesses")
{
    constexpr archlab::sim::Addr kBase = 0x8000'0000ULL;

    AddressSpace address_space;
    SimpleRam ram("dram0", 4096);
    address_space.map(kBase, 4096, ram);

    address_space.write64(kBase + 16, 0x0123'4567'89ab'cdefULL);
    ARCHLAB_CHECK_EQ(address_space.read64(kBase + 16), 0x0123'4567'89ab'cdefULL);
}

ARCHLAB_TEST("AddressSpace rejects overlapping regions")
{
    AddressSpace address_space;
    SimpleRam first("first", 4096);
    SimpleRam second("second", 4096);

    address_space.map(0x1000, 0x1000, first);
    ARCHLAB_CHECK_THROWS_AS(address_space.map(0x1800, 0x1000, second), std::invalid_argument);
}

ARCHLAB_TEST("AddressSpace rejects unmapped accesses")
{
    AddressSpace address_space;
    ARCHLAB_CHECK_THROWS_AS(address_space.read64(0xdead'beef), std::out_of_range);
}
