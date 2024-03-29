#pragma once

#include "hw/int.h"

i32
abs(i32 i) noexcept;

void
randSeed(i32 seed) noexcept;

void
randConsume(int count) noexcept;

/**
 * Random number from min to max, inclusive.
 */
i32
randRange(i32 min, i32 max) noexcept;

template<typename T>
static inline T
max(T a, T b) noexcept {
    return a > b ? a : b;
}

template<typename T>
static inline T
min(T a, T b) noexcept {
    return a < b ? a : b;
}
