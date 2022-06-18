#pragma once

#include "hw/int.h"

i32
abs(i32 i) noexcept;

void
randSeed(i32 seed) noexcept;

void
randConsume() noexcept;

/**
 * Random number from min to max, inclusive.
 */
i32
randRange(i32 min, i32 max) noexcept;

static inline int
max(int a, int b) noexcept {
    return a > b ? a : b;
}

static inline int
min(int a, int b) noexcept {
    return a < b ? a : b;
}
