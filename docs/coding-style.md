# Coding style

The repository-level `.clang-format` is authoritative.

## Naming

- files: `snake_case`;
- classes, structs, and enums: `PascalCase`;
- functions and local variables: `snake_case`;
- private data members: `snake_case_`;
- constants: `kPascalCase`;
- include guards: uppercase repository-relative path.

## Headers

Use standard include guards, never `#pragma once`:

```cpp
#ifndef ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_EXAMPLE_H_
#define ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_EXAMPLE_H_

// declarations

#endif  // ARCHLAB_RVSOC_SIM_INCLUDE_ARCHLAB_SIM_EXAMPLE_H_
```

Include order is:

1. the matching header in a `.cpp` file;
2. project headers;
3. standard-library headers.

## Change discipline

Formatting-only changes, refactoring, and behavior changes should normally be separate commits.
Every behavior change requires a test or an explicit explanation of why a test is not useful.
