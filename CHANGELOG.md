# Changelog

## Unreleased

### Added

- Added `EventStamp` ordering by tick, phase, delta, and sequence.
- Added registered tests covering every `EventStamp` ordering field.
- Added `ClockDomain` period, offset, edge detection, inclusive/strict edge queries, and overflow
  checks.
- Added a persistent project charter defining purpose, scope, boundaries, references, and design
  principles.
- Added a progress log that must be updated with meaningful implementation commits.

### In progress

- Milestone 0.2 simulation-time and event semantics.
- Migration of the callback event queue to explicit event timestamps.

## 0.1.0

- Established a clean C++20/CMake project baseline.
- Added a deterministic event queue and component foundation.
- Added address-space routing, RAM, magic MMIO, ROI statistics, and functional `SimpleCore`.
- Added tests, formatting rules, static-analysis configuration, helper scripts, and CI.
