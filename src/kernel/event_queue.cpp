#include "archlab/sim/kernel/event_queue.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

namespace archlab::sim {

bool EventQueue::EarlierEvent::operator()(const std::shared_ptr<Event>& lhs,
                                          const std::shared_ptr<Event>& rhs) const noexcept
{
    if (lhs->when != rhs->when) {
        return lhs->when > rhs->when;
    }
    return lhs->sequence > rhs->sequence;
}

EventQueue::EventId EventQueue::schedule_abs(Tick when, std::string name, Callback callback)
{
    if (when < now_) {
        throw std::invalid_argument("cannot schedule an event in the simulated past");
    }
    if (!callback) {
        throw std::invalid_argument("event callback must be valid");
    }

    const EventId event_id{next_event_id_++};
    auto event = std::make_shared<Event>(
        Event{when, next_sequence_++, event_id, std::move(name), std::move(callback), false});

    events_.push(event);
    live_events_.emplace(event_id.value, event);
    ++live_event_count_;
    return event_id;
}

EventQueue::EventId EventQueue::schedule_after(Tick delay, std::string name, Callback callback)
{
    if (delay > std::numeric_limits<Tick>::max() - now_) {
        throw std::overflow_error("event time overflow");
    }
    return schedule_abs(now_ + delay, std::move(name), std::move(callback));
}

bool EventQueue::cancel(EventId event_id)
{
    const auto iterator = live_events_.find(event_id.value);
    if (iterator == live_events_.end()) {
        return false;
    }

    iterator->second->cancelled = true;
    live_events_.erase(iterator);
    --live_event_count_;
    return true;
}

void EventQueue::run()
{
    while (!empty()) {
        execute_next_event();
    }
}

void EventQueue::run_until(Tick limit)
{
    if (limit < now_) {
        throw std::invalid_argument("run_until limit is earlier than the current time");
    }

    while (true) {
        const auto upcoming = next_tick();
        if (!upcoming.has_value() || *upcoming > limit) {
            break;
        }
        execute_next_event();
    }

    now_ = limit;
}

Tick EventQueue::now() const noexcept
{
    return now_;
}

bool EventQueue::empty() const noexcept
{
    return live_event_count_ == 0;
}

std::size_t EventQueue::size() const noexcept
{
    return live_event_count_;
}

std::optional<Tick> EventQueue::next_tick()
{
    discard_cancelled_events();
    if (events_.empty()) {
        return std::nullopt;
    }
    return events_.top()->when;
}

void EventQueue::enable_trace(bool enabled) noexcept
{
    trace_enabled_ = enabled;
}

void EventQueue::discard_cancelled_events()
{
    while (!events_.empty() && events_.top()->cancelled) {
        events_.pop();
    }
}

void EventQueue::execute_next_event()
{
    discard_cancelled_events();
    if (events_.empty()) {
        return;
    }

    auto event = events_.top();
    events_.pop();

    live_events_.erase(event->id.value);
    --live_event_count_;
    now_ = event->when;

    if (trace_enabled_) {
        std::clog << "[event] tick=" << now_ << " name=\"" << event->name << "\"\n";
    }

    event->callback();
}

}  // namespace archlab::sim
