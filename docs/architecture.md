# Architecture Baseline

See [project-charter.md](project-charter.md) for the project purpose and boundaries, and
[progress-log.md](progress-log.md) for the current implementation state.

## Current simulation semantics

The v0.1 callback `EventQueue` uses one global integer simulation time (`Tick`). Active callback
events are currently ordered by:

1. scheduled tick;
2. monotonically increasing insertion sequence.

Therefore, events with the same tick execute in insertion order and a run is repeatable when its
initial state and inputs are repeatable.

Milestone 0.2 has introduced an explicit `EventStamp` representation ordered by:

```text
tick -> phase -> delta -> sequence
```

The ordering type and its tests exist, but the callback `EventQueue` has not yet been fully migrated
to this richer timestamp. Until that migration is complete, code must not assume that every queued
callback already observes phase or delta semantics.

## Functional path

`SimpleCore` performs synchronous functional memory accesses through `AddressSpace`:

```text
SimpleCore -> AddressSpace -> MemoryDevice
```

This path exists to preserve a fast and obviously correct functional foundation. It must not be
silently reinterpreted as a timing or cycle-accurate path.

Future timing models will add explicit request/response ports and transactions alongside the
functional path.

## Component boundaries

- `EventQueue`: global time ordering and event execution.
- `Component`: identity and access to simulation-time services.
- `ClockDomain`: clock-edge calculations only; it does not manufacture unconditional clock events.
- `AddressSpace`: address decoding and routing only.
- `MemoryDevice`: functional memory-mapped device contract.
- `StatsManager`: total and ROI-scoped accounting.
- `MagicDevice`: software-visible MMIO control of statistics.
- `SimpleCore`: functional workload source, not an OOO core.

## Planned timing contract

The target kernel separates four concerns:

- `tick`: global simulated physical time;
- named phase: semantic ordering at one tick when an interface contract requires it;
- `delta`: zero-time causal propagation within a phase;
- `sequence`: deterministic software ordering when the earlier fields are equal.

A phase must not guess whether an asynchronous response met a receiver's clock edge. That decision
belongs to the receiving interface and clock-domain contract, for example by choosing between
`next_edge_after()` and `edge_at_or_after()`.

## Design rules

- Do not let C++ function-call order accidentally define hardware cycle semantics.
- Keep functional correctness separate from timing prediction.
- Add detail through replaceable models and explicit interfaces.
- Preserve determinism and test every time-ordering rule.
- Do not schedule events into the simulated past.
- Delayed events must validate ownership or identity before changing reusable resources.
- Record intentional differences from BOOM, gem5, XiangShan, or a specification.
- Keep the active implementation state distinct from planned architecture.