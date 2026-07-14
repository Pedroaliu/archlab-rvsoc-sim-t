# Roadmap

The roadmap defines the intended sequence, not a rigid deadline. Each milestone must end with a
buildable, tested, documented repository state.

See [project-charter.md](project-charter.md) for the long-term purpose and project boundaries, and
[progress-log.md](progress-log.md) for the current implementation state.

## Milestone 0.1.0 — project foundation (completed)

- clean CMake library/executable/test structure;
- deterministic callback event queue;
- component, address-space, RAM, magic MMIO, ROI statistics;
- functional `SimpleCore` baseline;
- formatting, static-analysis configuration, scripts, sanitizers, and CI.

## Milestone 0.2 — simulation time and event semantics (current)

Completed:

- explicit event timestamp representation;
- deterministic ordering by `tick -> phase -> delta -> sequence`;
- focused ordering tests.

In progress and remaining:

- `ClockDomain` period and phase-offset calculations;
- `next_edge_after()` and `edge_at_or_after()` boundary semantics;
- typed events in addition to compatibility callbacks;
- component identifiers;
- scheduling-to-the-past checks;
- zero-delay-loop protection;
- event trace and checkpoint-friendly event representation;
- clocked-component active/idle wakeup behavior.

Exit criteria:

- all time-ordering rules have tests;
- multiple clock domains can share one global event queue;
- idle components do not require unconditional per-cycle polling;
- replay from the same initial state produces the same event order.

## Milestone 0.3 — ports and transactions

- request/response ports;
- transaction identity, ownership, completion, and stale-response handling;
- fixed-latency memory service;
- queue capacity, backpressure, retry, and latency statistics;
- retain a separate functional access path;
- first end-to-end `TrafficGenerator -> Queue -> Memory -> Completion` experiment.

## Milestone 0.4 — dynamic instruction lifecycle

- static instruction versus dynamic instruction separation;
- BOOM-inspired stages from fetch through commit;
- per-stage lifecycle trace from fetch to commit or squash;
- pipeline current-state/next-state update semantics;
- selective trace enablement by instruction, PC, or ROI;
- no claim of OOO accuracy yet.

## Milestone 0.5 — first BOOM-like OOO baseline

- rename map, free list, busy table, and physical registers;
- ROB and in-order commit;
- issue queue, wakeup/select, and functional units;
- branch checkpoint, squash, and precise exceptions;
- delayed-event identity checks for squashed instructions;
- commit-level differential testing against a reference model.

## Later processor milestones

- load/store queue and store-to-load forwarding;
- memory-dependence prediction, replay, and ordering violations;
- branch prediction and frontend decoupling;
- advanced paper-inspired predictors and execution mechanisms;
- calibration against BOOM, XiangShan, gem5 O3, or XS-GEM5 where appropriate.

## Later SoC milestones

- cache hierarchy and coherence;
- NoC routers, virtual channels, buffering, credits, and routing;
- memory controller and DDR timing abstraction;
- PCIe Root Complex, DMA, interrupts, IOMMU, ATS, and PASID;
- platform devices and full-system Linux boot;
- RISC-V Hypervisor extension, KVM, QEMU, and an L2 guest;
- QEMU/Spike/NEMU/trace/SystemC/RTL adapters;
- dynamic functional, timing, transaction, and cycle-detail switching.

## Deferred work

The following are deliberately deferred until the sequential simulator and validation framework are
mature:

- detailed GPU/NPU/DPU internal models;
- optimistic or distributed parallel discrete-event simulation;
- transistor, analog, PHY, or SerDes simulation;
- exact PPA modeling;
- production-grade compatibility or hypervisor behavior.