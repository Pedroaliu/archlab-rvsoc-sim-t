#include "archlab/sim/cpu/simple_core.h"
#include "archlab/sim/devices/magic_device.h"
#include "archlab/sim/devices/simple_ram.h"
#include "archlab/sim/kernel/event_queue.h"
#include "archlab/sim/platform/address_space.h"
#include "archlab/sim/stats/stats_manager.h"
#include "archlab/sim/types.h"

#include <iomanip>
#include <iostream>

int main()
{
    using namespace archlab::sim;

    constexpr Addr kDramBase = 0x8000'0000ULL;
    constexpr Addr kMagicBase = 0x4000'0000ULL;
    constexpr Addr kRegionSize = 4096;

    std::cout << "ArchLab RVSoC-Sim v0.1.0 - project foundation\n";

    EventQueue event_queue;
    StatsManager stats;
    AddressSpace address_space;
    SimpleRam dram("dram0", kRegionSize);
    MagicDevice magic("magic0", event_queue, stats);
    SimpleCore core("core0", event_queue, address_space, stats);

    address_space.map(kDramBase, kRegionSize, dram);
    address_space.map(kMagicBase, kRegionSize, magic);

    static_cast<void>(event_queue.schedule_abs(100, "reset statistics", [&]() {
        address_space.write64(kMagicBase, MagicDevice::kResetStats);
    }));

    static_cast<void>(event_queue.schedule_abs(120, "begin ROI", [&]() {
        address_space.write64(kMagicBase, MagicDevice::kBeginRoi);
    }));

    static_cast<void>(event_queue.schedule_abs(130, "functional workload phase 1", [&]() {
        core.alu(9);
        core.store64(kDramBase, 0x1122'3344'5566'7788ULL);
    }));

    static_cast<void>(event_queue.schedule_abs(160, "functional workload phase 2", [&]() {
        core.alu(16);
        const Data value = core.load64(kDramBase);
        core.branch(true);
        core.branch(false);
        core.branch(true);

        std::cout << "[workload] read dram value=0x" << std::hex << value << std::dec << '\n';
    }));

    static_cast<void>(event_queue.schedule_abs(190, "end ROI", [&]() {
        address_space.write64(kMagicBase, MagicDevice::kEndRoi);
    }));

    static_cast<void>(event_queue.schedule_abs(200, "dump statistics", [&]() {
        address_space.write64(kMagicBase, MagicDevice::kDumpStats);
    }));

    event_queue.run_until(250);
    std::cout << "Simulation finished at tick " << event_queue.now() << '\n';

    return 0;
}
