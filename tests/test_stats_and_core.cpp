#include "test_framework.h"

#include "archlab/sim/cpu/simple_core.h"
#include "archlab/sim/devices/magic_device.h"
#include "archlab/sim/devices/simple_ram.h"
#include "archlab/sim/kernel/event_queue.h"
#include "archlab/sim/platform/address_space.h"
#include "archlab/sim/stats/stats_manager.h"

using namespace archlab::sim;

ARCHLAB_TEST("SimpleCore records functional work inside an ROI")
{
    constexpr Addr kDramBase = 0x8000'0000ULL;

    EventQueue queue;
    StatsManager stats;
    AddressSpace address_space;
    SimpleRam ram("dram0", 4096);
    SimpleCore core("core0", queue, address_space, stats);

    address_space.map(kDramBase, 4096, ram);

    stats.begin_roi(10);
    core.alu(5);
    core.store64(kDramBase, 0xaabb'ccdd'eeff'0011ULL);
    ARCHLAB_CHECK_EQ(core.load64(kDramBase), 0xaabb'ccdd'eeff'0011ULL);
    core.branch(true);
    stats.end_roi(20);

    const StatsManager::Snapshot snapshot = stats.snapshot(20);
    ARCHLAB_CHECK_EQ(snapshot.roi_cycles, 10U);
    ARCHLAB_CHECK_EQ(snapshot.roi.instructions, 8U);
    ARCHLAB_CHECK_EQ(snapshot.roi.alu_operations, 5U);
    ARCHLAB_CHECK_EQ(snapshot.roi.loads, 1U);
    ARCHLAB_CHECK_EQ(snapshot.roi.stores, 1U);
    ARCHLAB_CHECK_EQ(snapshot.roi.branches, 1U);
    ARCHLAB_CHECK_EQ(snapshot.roi.branches_taken, 1U);
}

ARCHLAB_TEST("MagicDevice controls ROI state through MMIO")
{
    constexpr Addr kMagicBase = 0x4000'0000ULL;

    EventQueue queue;
    StatsManager stats;
    AddressSpace address_space;
    MagicDevice magic("magic0", queue, stats);

    address_space.map(kMagicBase, 4096, magic);

    static_cast<void>(queue.schedule_abs(10, "begin", [&]() {
        address_space.write64(kMagicBase, MagicDevice::kBeginRoi);
    }));
    static_cast<void>(queue.schedule_abs(20, "end", [&]() {
        address_space.write64(kMagicBase, MagicDevice::kEndRoi);
    }));

    queue.run_until(20);

    const StatsManager::Snapshot snapshot = stats.snapshot(queue.now());
    ARCHLAB_CHECK(!snapshot.roi_active);
    ARCHLAB_CHECK_EQ(snapshot.roi_cycles, 10U);
    ARCHLAB_CHECK_EQ(address_space.read64(kMagicBase), MagicDevice::kEndRoi);
}
