# Changelog

## Unreleased

### Added

- Added `EventStamp` ordering by tick, phase, delta, and sequence.
- Added focused tests for timestamp ordering semantics.
- Added a persistent project charter defining purpose, scope, boundaries, references, and design
  principles.
- Added a progress log that must be updated with meaningful implementation commits.

### In progress

- Milestone 0.2 simulation-time and event semantics.
- `ClockDomain` period, offset, and edge calculations.

## 0.1.0

- Established a clean C++20/CMake project baseline.
- Added a deterministic event queue and component foundation.
- Added address-space routing, RAM, magic MMIO, ROI statistics, and functional `SimpleCore`.
- Added tests, formatting rules, static-analysis configuration, helper scripts, and CI.