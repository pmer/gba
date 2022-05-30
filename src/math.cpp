#include "math.h"

s32
abs(s32 i) noexcept {
    s32 mask = i >> 31;
    return (i ^ mask) - mask;
}

static s32 seed = 42;

static s32
rand_() noexcept {
    seed = 1664525 * seed + 1013904223;
    return (seed >> 16) & 0x7FFF;
}

void
randSeed(s32 seed_) noexcept {
    seed = seed_;
}

s32
randRange(s32 min, s32 max) noexcept {
    //return rand_() % (max - min + 1) + min;
    return (rand_() * (max - min + 1) >> 15) + min;
}
