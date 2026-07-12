# Roadmap

## Milestone 0.1.0 — project foundation (current)

- clean CMake library/executable/test structure;
- deterministic callback event queue;
- component, address-space, RAM, magic MMIO, ROI statistics;
- functional `SimpleCore` baseline;
- formatting, static-analysis configuration, scripts, and CI.

## Milestone 0.2 — typed simulation kernel

- explicit `SimTime` and clock-domain conversion;
- typed events in addition to compatibility callbacks;
- component identifiers;
- event trace and zero-delay-loop protection;
- checkpoint-friendly event representation.

## Milestone 0.3 — ports and transactions

- request/response ports;
- transaction identity and completion;
- fixed-latency memory service;
- queue capacity, backpressure, retry, and latency statistics;
- retain a separate functional access path.

## Milestone 0.4 — dynamic instruction lifecycle

- static instruction versus dynamic instruction separation;
- BOOM-inspired stages from fetch through commit;
- per-stage lifecycle trace;
- pipeline current-state/next-state update semantics;
- no claim of OOO accuracy yet.

## Milestone 0.5 — first BOOM-like OOO baseline

- rename map, free list, busy table, physical registers;
- ROB and in-order commit;
- issue queue, wakeup/select, and functional units;
- branch checkpoint, squash, and precise exceptions;
- commit-level differential testing against a reference model.

Later milestones add LSU, caches, coherence, NoC, DDR, PCIe, IOMMU, full-system software, and
RISC-V virtualization.
