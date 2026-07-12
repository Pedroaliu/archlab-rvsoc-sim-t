# Contributing

Contributions for learning, architecture experiments, tests, and documentation are welcome.

Before a large architectural change, open an issue describing:

- the modeled hardware or simulation concept;
- functional and timing semantics;
- resource limits and backpressure;
- flush, cancellation, or recovery behavior;
- validation plan and reference implementation/specification.

Run before submitting:

```bash
./scripts/format.sh
./scripts/check.sh
```

Do not mix broad formatting changes with behavior changes in the same commit.
