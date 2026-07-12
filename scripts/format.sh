#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${repo_root}"

if ! command -v clang-format >/dev/null 2>&1; then
    echo "error: clang-format is not installed" >&2
    exit 1
fi

find app include src tests \
    -type f \
    \( -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) \
    -print0 | sort -z | xargs -0 clang-format -i
