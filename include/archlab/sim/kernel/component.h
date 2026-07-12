#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_KERNEL_COMPONENT_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_KERNEL_COMPONENT_H_

#include "archlab/sim/kernel/event_queue.h"

#include <string>
#include <string_view>
#include <utility>

namespace archlab::sim {

class Component {
public:
    Component(std::string name, EventQueue& event_queue)
        : name_(std::move(name)), event_queue_(event_queue)
    {
    }

    virtual ~Component() = default;

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    Component(Component&&) = delete;
    Component& operator=(Component&&) = delete;

    [[nodiscard]] std::string_view name() const noexcept
    {
        return name_;
    }

protected:
    [[nodiscard]] EventQueue& event_queue() noexcept
    {
        return event_queue_;
    }

    [[nodiscard]] const EventQueue& event_queue() const noexcept
    {
        return event_queue_;
    }

private:
    std::string name_;
    EventQueue& event_queue_;
};

}  // namespace archlab::sim

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_KERNEL_COMPONENT_H_
