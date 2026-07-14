# Progress Log

This file is the persistent project memory for implementation progress. It records what changed,
why it changed, how it was validated, and what comes next.

## Update rule

Before every meaningful implementation commit:

1. add or update the current entry in this file;
2. record the milestone, completed behavior, tests, and next step;
3. commit the code and progress-log update together.

A useful entry answers four questions:

- What behavior now exists?
- Why was it added this way?
- How was it verified?
- What is the next smallest step?

Pure typo or formatting-only commits may be grouped into the nearest project-state entry, but any
commit that changes simulator semantics, architecture, interfaces, roadmap, or milestone status must
update this log.

## Current status

**Milestone 0.2 — simulation time and event semantics**

Completed:

- clean v0.1.0 project foundation;
- deterministic callback event queue baseline;
- `EventStamp` ordering by `tick -> phase -> delta -> sequence`;
- focused tests for phase, delta, and sequence ordering.

In progress:

- `ClockDomain` with period, offset, `next_edge_after()`, and `edge_at_or_after()`.

Next:

- finish and test `ClockDomain`;
- migrate `EventQueue` to explicit event-time semantics;
- define scheduling-to-the-past checks;
- add clocked-component wake/sleep behavior.

## Commit history

### `65e9631` — Event timestamp ordering

Milestone: 0.2

What changed:

- added simulation-time types and `EventStamp`;
- established deterministic lexicographic ordering by tick, phase, delta, and sequence;
- added tests proving that earlier fields dominate later fields.

Why:

- the whole SoC needs one explicit and testable ordering rule;
- later clock domains, delayed responses, and cycle models depend on deterministic event ordering.

Validation:

- project compiled successfully;
- all unit tests passed.

Next step:

- implement `ClockDomain` edge calculations and boundary cases.

### `46a591e` — Project foundation v0.1.0

Milestone: 0.1.0

What changed:

- established the C++20/CMake library, executable, and test structure;
- added the deterministic event queue, components, address space, RAM, magic MMIO, ROI statistics,
  and functional `SimpleCore`;
- added formatting, static analysis, sanitizers, helper scripts, CI, and documentation.

Why:

- preserve the useful ideas from the earlier lab repository while starting a clean long-lived
  research project;
- create a tested functional baseline before adding timing complexity.

Validation:

- debug build passed with strict warnings;
- unit tests passed;
- ASan/UBSan build and tests passed.

Next step:

- make simulation time, event identity, and clock-domain semantics explicit.

## Entry template

Copy this section before the next meaningful commit:

```markdown
### `<short-sha>` — <commit title>

Milestone: <milestone>

What changed:

- ...

Why:

- ...

Validation:

- build command/result;
- tests added or executed;
- differential/reference checks when applicable.

Next step:

- ...
```

When preparing a commit locally, use `pending` instead of the SHA. Replace it with the short SHA in
the following progress-maintenance commit only when exact historical linkage is important. Normally
the entry title and the Git history already make the association clear.