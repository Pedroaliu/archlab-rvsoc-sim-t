# ArchLab RVSoC-Sim

A clean, research-oriented foundation for learning and experimenting with RISC-V CPU and
SoC simulation.

This repository starts from **Milestone 0.1.0: Project Foundation**. The earlier lab-style
repository remains useful as learning history, but this project is organized as a long-lived
software project from the beginning.

## Long-term goal

Build an extensible full-system simulator that can eventually support:

- functional, timing, transaction-level, and cycle-accurate models;
- dynamic switching of simulation detail by ROI, PC range, process, VM, or trigger;
- a BOOM-like out-of-order baseline and experiments with advanced architecture papers;
- cache coherence, NoC, DDR controller, PCIe Root Complex, IOMMU, and interrupts;
- Linux, the RISC-V Hypervisor extension, KVM, QEMU, and an L2 guest;
- QEMU, Spike/NEMU, trace, synthetic, SystemC, and RTL frontends/backends;
- differential testing, checkpoints, replay, statistics, and lifecycle traces.

GPU, NPU, and DPU internals are intentionally outside the main implementation scope; they can
be connected through external simulator adapters later.

## Current milestone

Milestone 0.1.0 provides a tested project baseline:

- deterministic event queue;
- component base class;
- memory-mapped address space;
- simple little-endian RAM;
- ROI and operation statistics;
- MMIO magic device;
- a deliberately functional `SimpleCore`;
- CMake presets, tests, formatting, static-analysis configuration, scripts, and CI.

This milestone is **not** a performance predictor. It establishes the software and simulation
semantics on which later timing and cycle models will be built.

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

CLion will detect the repository-level `.clang-format`. Use **Code → Reformat Code** or
`Ctrl+Alt+L`.

All project headers use standard `#ifndef/#define/#endif` include guards. The project does not
mix them with `#pragma once`.

## Layout

```text
app/                executable demo
cmake/              shared CMake helpers
docs/               architecture, roadmap, and coding rules
include/archlab/     public headers
src/                 library implementation
tests/               dependency-free unit tests
scripts/             format and verification helpers
```

## License

Apache License 2.0.
