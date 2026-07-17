#include "archlab/sim/kernel/event_queue.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

namespace archlab::sim {

bool EventQueue::EarlierEvent::operator()(const std::shared_ptr<Event>& lhs,
                                           const std::shared_ptr<Event>& rhs) const noexcept
{
    return lhs->stamp > rhs->stamp;
}

EventQueue::EventId EventQueue::schedule_abs(Tick when, std::string name, Callback callback)
{
    return schedule_abs(
        when, EventPhase::Input, Delta{0}, std::move(name), std::move(callback));
}

EventQueue::EventId EventQueue::schedule_abs(Tick when,
                                              EventPhase phase,
                                              Delta delta,
                                              std::string name,
                                              Callback callback)
{
    if (!callback) {
        throw std::invalid_argument("event callback must be valid");
    }

    const EventStamp stamp{
        .tick = when,
        .phase = phase,
        .delta = delta,
        .sequence = next_sequence_,
    };
    validate_schedule_stamp(stamp);

    const EventId event_id{next_event_id_++};
    auto event = std::make_shared<Event>(Event{
        .stamp = stamp,
        .id = event_id,
        .name = std::move(name),
        .callback = std::move(callback),
        .cancelled = false,
    });
    ++next_sequence_;

    events_.push(event);
    live_events_.emplace(event_id.value, event);
    ++live_event_count_;
    return event_id;
}

EventQueue::EventId EventQueue::schedule_after(Tick delay,
                                                std::string name,
                                                Callback callback)
{
    return schedule_after(
        delay, EventPhase::Input, Delta{0}, std::move(name), std::move(callback));
}

EventQueue::EventId EventQueue::schedule_after(Tick delay,
                                                EventPhase phase,
                                                Delta delta,
                                                std::string name,
                                                Callback callback)
{
    if (delay > std::numeric_limits<Tick>::max() - now_) {
        throw std::overflow_error("event time overflow");
    }
    return schedule_abs(
        now_ + delay, phase, delta, std::move(name), std::move(callback));
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

    if (limit > now_) {
        now_ = limit;
        last_executed_stamp_.reset();
    }
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
    return events_.top()->stamp.tick;
}

void EventQueue::enable_trace(bool enabled) noexcept
{
    trace_enabled_ = enabled;
}

void EventQueue::validate_schedule_stamp(const EventStamp& stamp) const
{
    if (stamp.tick < now_) {
        throw std::invalid_argument("cannot schedule an event in the simulated past");
    }

    if (last_executed_stamp_.has_value() && stamp.tick == now_ &&
        last_executed_stamp_->tick == now_ && stamp < *last_executed_stamp_) {
        throw std::invalid_argument(
            "cannot schedule an event before the current phase or delta");
    }
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
    now_ = event->stamp.tick;
    last_executed_stamp_ = event->stamp;

    if (trace_enabled_) {
        std::clog << "[event] tick=" << event->stamp.tick
                  << " phase=" << static_cast<unsigned int>(event->stamp.phase)
                  << " delta=" << event->stamp.delta
                  << " sequence=" << event->stamp.sequence << " name=\"" << event->name
                  << "\"\n";
    }

    event->callback();
}

}  // namespace archlab::sim
