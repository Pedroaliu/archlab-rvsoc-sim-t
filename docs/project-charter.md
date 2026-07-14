# Project Charter

## Purpose

ArchLab RVSoC-Sim is a learning-first, research-oriented simulator for studying modern computer
architecture and complete RISC-V SoC behavior.

The project exists to turn architecture theory into executable models. It should make it possible
to understand not only whether software runs, but also how instructions, requests, queues, clocks,
and devices interact across a complete system.

The simulator is intentionally free from commercial compatibility baggage. This allows the project
to adopt clean interfaces, experiment with advanced research ideas, and change internal designs when
a better learning or modeling approach is found.

## Long-term target

The final platform should be able to:

- run bare-metal programs and a full Linux system;
- support the RISC-V Hypervisor extension;
- run Linux + KVM + QEMU inside the simulated SoC and boot an L2 guest;
- model CPU microarchitecture, cache hierarchy and coherence, NoC, DDR controller, PCIe Root
  Complex, IOMMU, DMA, timers, and interrupts;
- support functional, timing, transaction-level, and cycle-accurate models;
- switch modeling detail by ROI, time window, PC range, process, VM, instruction class, component,
  or trigger;
- trace the lifecycle of selected dynamic instructions from fetch to commit or squash;
- connect multiple execution frontends and reference models, including an internal interpreter,
  QEMU, Spike/NEMU, trace replay, synthetic traffic, SystemC, and RTL;
- provide differential testing, checkpoints, deterministic replay, statistics, and debugging traces;
- evaluate BOOM-like baselines and experimental ideas from architecture papers.

## Main implementation scope

The project will implement and study the following areas directly:

### Simulation infrastructure

- global simulation time and clock domains;
- deterministic event scheduling;
- typed events, ports, links, and transactions;
- functional and timing access paths;
- model switching, ROI, triggers, checkpoints, replay, and tracing;
- testable component interfaces and explicit state-transition semantics.

### Processor and memory system

- RISC-V functional execution and architectural state;
- in-order and BOOM-inspired out-of-order cores;
- rename, ROB, issue, functional units, LSQ, speculation, recovery, and precise exceptions;
- branch prediction, prefetching, caches, TLBs, and coherence;
- advanced microarchitecture experiments from research papers.

### SoC and platform

- NoC routers, virtual channels, buffering, credits, routing, and arbitration;
- memory controllers and abstract DDR timing constraints;
- PCIe transaction, topology, bandwidth, credit, DMA, interrupt, ATS/PASID, and IOMMU behavior;
- platform devices required to boot and run system software;
- RISC-V privilege and virtualization mechanisms.

## Explicit boundaries

The following areas are outside the main implementation scope unless a later milestone explicitly
changes the decision:

- detailed GPU, NPU, or DPU internal microarchitecture;
- a production-quality commercial hypervisor or compatibility product;
- analog, circuit, SerDes, PHY waveform, signal-integrity, or transistor-level simulation;
- exact physical-design PPA prediction;
- a complete PCIe electrical/link training model;
- a real-server performance-diagnosis product or monitoring agent;
- distributed or optimistic parallel discrete-event simulation in early milestones;
- reimplementing mature external simulators when an adapter is sufficient.

GPU, NPU, DPU, network, and RTL models may be connected through external simulator adapters. The
project should study their software-visible workflows, topology, data movement, virtualization, and
integration without rebuilding every device internally.

## Feasibility rule for paper experiments

The simulator does not need sign-off-quality PPA, but architecture experiments must not rely on
magic hardware. Each experimental structure should declare relevant limits such as:

- number of entries and stored bits;
- read/write ports and per-cycle bandwidth;
- lookup and update latency;
- queue capacity and backpressure;
- pipeline placement and recovery behavior.

This is the project's lightweight architectural feasibility envelope.

## Reference strategy

Different references serve different purposes:

- Spike or NEMU: ISA and architectural-state correctness;
- BOOM: primary baseline for understandable OOO hardware semantics;
- XiangShan: modern high-performance design reference;
- gem5 O3 and XS-GEM5: software simulation structure, calibration, and statistics;
- QEMU: fast functional execution and full-system integration;
- SystemC/TLM and external simulators: component and device co-simulation.

References guide the model, but the project must document intentional simplifications and deviations.

## Design principles

1. Correctness before detail.
2. Explicit time and causality before optimization.
3. Functional correctness and timing prediction are separate contracts.
4. C++ call order must not accidentally define hardware behavior.
5. Components communicate through explicit interfaces rather than hidden shared state.
6. Every reusable resource, queue, and delayed event needs clear ownership and cancellation rules.
7. Every timing rule and recovery rule must have a focused test.
8. Determinism, checkpointing, and differential testing are first-class features.
9. Model detail is selected by the research question, not by a desire to model everything.
10. Each milestone must leave the repository buildable, tested, and explainable.

## Learning method

Development follows a Feynman-style loop:

1. explain why a mechanism is needed in plain language;
2. predict what fails without it;
3. build the smallest experiment that exposes the behavior;
4. explain the result in the learner's own words;
5. implement the production version only after the semantics are clear.

The project is both a simulator and a structured computer-architecture laboratory.