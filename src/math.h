#pragma once

extern "C" {
#include <tonc_types.h>
}

s32
abs(s32 i) noexcept;

void
randSeed(s32 seed) noexcept;

/**
 * Random number from min to max, inclusive.
 */
s32
randRange(s32 min, s32 max) noexcept;
