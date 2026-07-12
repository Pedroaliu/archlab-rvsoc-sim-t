#include "archlab/sim/devices/simple_ram.h"

#include <limits>
#include <stdexcept>
#include <utility>

namespace archlab::sim {

SimpleRam::SimpleRam(std::string name, Addr size) : name_(std::move(name))
{
    if (size == 0) {
        throw std::invalid_argument("RAM size must be non-zero");
    }
    if (size > std::numeric_limits<std::size_t>::max()) {
        throw std::length_error("RAM size cannot be represented by the host");
    }

    bytes_.resize(static_cast<std::size_t>(size));
}

Data SimpleRam::read64(Addr offset)
{
    check_access(offset, sizeof(Data));

    Data value = 0;
    for (Addr index = 0; index < sizeof(Data); ++index) {
        const auto byte = bytes_.at(static_cast<std::size_t>(offset + index));
        value |= static_cast<Data>(byte) << (index * 8);
    }
    return value;
}

void SimpleRam::write64(Addr offset, Data value)
{
    check_access(offset, sizeof(Data));

    for (Addr index = 0; index < sizeof(Data); ++index) {
        bytes_.at(static_cast<std::size_t>(offset + index)) =
            static_cast<std::uint8_t>((value >> (index * 8)) & 0xffU);
    }
}

std::string_view SimpleRam::device_name() const noexcept
{
    return name_;
}

Addr SimpleRam::size() const noexcept
{
    return static_cast<Addr>(bytes_.size());
}

void SimpleRam::check_access(Addr offset, Addr access_size) const
{
    const Addr memory_size = size();
    if (offset > memory_size || access_size > memory_size - offset) {
        throw std::out_of_range("RAM access is outside the device range");
    }
}

}  // namespace archlab::sim
