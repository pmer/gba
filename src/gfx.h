#pragma once

extern "C" {
#include <tonc_types.h>
}

/**
 * Set GBA rendering context to mode 3 with bitmap rendering.
 */
void
mode3() noexcept;

/**
 * Draw rectangle from left, top (inclusive) with width, height (exclusive).
 */
void
drawRect(s32 left, s32 top, s32 width, s32 height, COLOR color) noexcept;

/**
 * Draw line from x, y (inclusive) to x2, y2 (inclusive).
 */
void
drawLine(s32 x, s32 y, s32 x2, s32 y2, COLOR color) noexcept;

void
vsync() noexcept;
