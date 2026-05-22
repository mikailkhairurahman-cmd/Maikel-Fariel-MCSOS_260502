#!/usr/bin/env bash

set -euo pipefail

mkdir -p build/meta

OUT="build/meta/toolchain-versions.txt"

{
    echo "date_utc=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
    echo "git=$(git --version)"
    echo "make=$(make --version | head -n1)"
    echo "clang=$(clang --version | head -n1)"
    echo "ld.lld=$(ld.lld --version | head -n1)"
    echo "qemu=$(qemu-system-x86_64 --version | head -n1)"
    echo "python=$(python3 --version)"
} > "$OUT"

echo "OK: metadata generated -> $OUT"
