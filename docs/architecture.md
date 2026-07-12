# Architecture baseline

## Current simulation semantics

Milestone 0.1 uses one global integer simulation time (`Tick`) and one deterministic event
queue. Events are ordered by:

1. scheduled tick;
2. monotonically increasing insertion sequence.

Therefore, events with the same tick execute in insertion order and a run is repeatable when
its initial state and inputs are repeatable.

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
- `Component`: identity and access to simulation time services.
- `AddressSpace`: address decoding and routing only.
- `MemoryDevice`: functional memory-mapped device contract.
- `StatsManager`: total and ROI-scoped accounting.
- `MagicDevice`: software-visible MMIO control of statistics.
- `SimpleCore`: functional workload source, not an OOO core.

## Design rules

- Do not let C++ function-call order accidentally define hardware cycle semantics.
- Keep functional correctness separate from timing prediction.
- Add detail through replaceable models and explicit interfaces.
- Preserve determinism and test every time-ordering rule.
- Record intentional differences from BOOM, gem5, XiangShan, or a specification.
