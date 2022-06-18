#include "math.h"

#include "hw/int.h"

i32
abs(i32 i) noexcept {
    i32 mask = i >> 31;
    return (i ^ mask) - mask;
}

static i32 seed = 42;

static i32
rand_() noexcept {
    seed = 1664525 * seed + 1013904223;
    return (seed >> 16) & 0x7fff;
}

void
randConsume() noexcept {
    rand_();
}

void
randSeed(i32 seed_) noexcept {
    seed = seed_;
}

i32
randRange(i32 min, i32 max) noexcept {
    //return rand_() % (max - min + 1) + min;
    return (rand_() * (max - min + 1) >> 15) + min;
}
