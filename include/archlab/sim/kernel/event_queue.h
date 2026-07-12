#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_KERNEL_EVENT_QUEUE_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_KERNEL_EVENT_QUEUE_H_

#include "archlab/sim/types.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

namespace archlab::sim {

class EventQueue {
public:
    using Callback = std::function<void()>;

    struct EventId {
        std::uint64_t value{};

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return value != 0;
        }
    };

    EventQueue() = default;

    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;
    EventQueue(EventQueue&&) = delete;
    EventQueue& operator=(EventQueue&&) = delete;

    [[nodiscard]] EventId schedule_abs(Tick when, std::string name, Callback callback);
    [[nodiscard]] EventId schedule_after(Tick delay, std::string name, Callback callback);
    [[nodiscard]] bool cancel(EventId event_id);

    void run();
    void run_until(Tick limit);

    [[nodiscard]] Tick now() const noexcept;
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] std::optional<Tick> next_tick();

    void enable_trace(bool enabled) noexcept;

private:
    struct Event {
        Tick when{};
        std::uint64_t sequence{};
        EventId id{};
        std::string name;
        Callback callback;
        bool cancelled{};
    };

    struct EarlierEvent {
        [[nodiscard]] bool operator()(const std::shared_ptr<Event>& lhs,
                                      const std::shared_ptr<Event>& rhs) const noexcept;
    };

    void discard_cancelled_events();
    void execute_next_event();

    Tick now_{};
    std::uint64_t next_sequence_{};
    std::uint64_t next_event_id_{1};
    bool trace_enabled_{};
    std::size_t live_event_count_{};

    std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EarlierEvent>
        events_;
    std::unordered_map<std::uint64_t, std::shared_ptr<Event>> live_events_;
};

}  // namespace archlab::sim

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_KERNEL_EVENT_QUEUE_H_
