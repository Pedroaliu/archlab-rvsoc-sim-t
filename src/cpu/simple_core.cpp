#include "archlab/sim/cpu/simple_core.h"

#include <utility>

namespace archlab::sim {

SimpleCore::SimpleCore(std::string name,
                       EventQueue& event_queue,
                       AddressSpace& address_space,
                       StatsManager& stats)
    : Component(std::move(name), event_queue), address_space_(address_space), stats_(stats)
{
}

void SimpleCore::alu(std::uint64_t operation_count) noexcept
{
    stats_.record_alu(operation_count);
}

Data SimpleCore::load64(Addr address)
{
    const Data value = address_space_.read64(address);
    stats_.record_load();
    return value;
}

void SimpleCore::store64(Addr address, Data value)
{
    address_space_.write64(address, value);
    stats_.record_store();
}

void SimpleCore::branch(bool taken) noexcept
{
    stats_.record_branch(taken);
}

}  // namespace archlab::sim
