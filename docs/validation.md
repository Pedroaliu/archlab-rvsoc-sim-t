# Validation record

Milestone 0.1.0 was validated with:

- CMake 3.31.6;
- GCC 14.2.0;
- C++20;
- project warnings enabled and promoted to errors;
- AddressSanitizer and UndefinedBehaviorSanitizer.

Validated commands:

```bash
cmake --preset debug -DARCHLAB_WARNINGS_AS_ERRORS=ON
cmake --build --preset debug
ctest --preset debug

cmake --preset asan -DARCHLAB_WARNINGS_AS_ERRORS=ON
cmake --build --preset asan
ctest --preset asan
```

All eight unit tests passed in both the normal Debug and sanitizer builds.
