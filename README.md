# ArchLab RVSoC-Sim

A clean, learning-first, research-oriented foundation for studying modern RISC-V CPU and complete
SoC simulation.

This repository starts from **Milestone 0.1.0: Project Foundation**. The earlier lab-style repository
remains useful as learning history, but this project is organized as a long-lived software project
from the beginning.

## Purpose

Build an extensible full-system simulator that can eventually support:

- functional, timing, transaction-level, and cycle-accurate models;
- dynamic switching of simulation detail by ROI, PC range, process, VM, instruction class, component,
  or trigger;
- a BOOM-like out-of-order baseline and experiments with advanced architecture papers;
- cache coherence, NoC, DDR controller, PCIe Root Complex, IOMMU, DMA, and interrupts;
- Linux, the RISC-V Hypervisor extension, KVM, QEMU, and an L2 guest;
- QEMU, Spike/NEMU, trace, synthetic, SystemC, and RTL frontends/backends;
- differential testing, checkpoints, replay, statistics, and instruction lifecycle traces.

GPU, NPU, and DPU internals are intentionally outside the main implementation scope. They may be
connected through external simulator adapters while this project focuses on CPU, SoC, full-system,
and virtualization mechanisms.

The complete purpose, implementation scope, explicit non-goals, reference strategy, and design
principles are defined in [docs/project-charter.md](docs/project-charter.md).

## Current milestone

**Milestone 0.2 — simulation time and event semantics**

Completed so far:

- clean v0.1.0 project foundation;
- deterministic callback event queue baseline;
- explicit `EventStamp` representation;
- deterministic ordering by `tick -> phase -> delta -> sequence`;
- registered ordering tests;
- `ClockDomain` period, offset, edge detection, inclusive/strict edge queries, and overflow checks.

In progress:

- migration of the callback event queue to explicit event timestamps.

See:

- [docs/roadmap.md](docs/roadmap.md) for milestone sequencing;
- [docs/progress-log.md](docs/progress-log.md) for the persistent current state;
- [docs/architecture.md](docs/architecture.md) for implemented simulation semantics.

## Progress-log rule

Every meaningful implementation commit must update `docs/progress-log.md` in the same commit. The
entry records:

- what behavior now exists;
- why the design was chosen;
- how it was validated;
- the next smallest step.

This keeps project continuity in the repository instead of depending on chat history or memory.

## Build

Requirements: CMake 3.21+, a C++20 compiler, and Ninja when using the presets.

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
./build/debug/app/archlab-sim
```

A portable build without presets also works:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j"$(nproc)"
ctest --test-dir build --output-on-failure
```

## Format

```bash
./scripts/format.sh
```

CLion will detect the repository-level `.clang-format`. Use **Code -> Reformat Code** or
`Ctrl+Alt+L`.

All project headers use standard `#ifndef/#define/#endif` include guards. The project does not mix
them with `#pragma once`.

## Layout

```text
app/                executable demo
cmake/              shared CMake helpers
docs/               charter, architecture, roadmap, progress, and coding rules
include/archlab/     public headers
src/                 library implementation
tests/               dependency-free unit tests
scripts/             format and verification helpers
```

## Development method

The project uses small, testable steps and a Feynman-style learning loop:

1. explain why the mechanism is needed;
2. predict what fails without it;
3. build the smallest experiment;
4. explain the observed behavior;
5. implement the production version.

## License

Apache License 2.0.
