#include "print.h"

#include <tonc.h>

static void
notePlay(int note, int octave) noexcept {
    REG_SND1FREQ = SFREQ_RESET | SND_RATE(note, octave);
}

static void
sos() noexcept {
    const u8 lens[6] = {1, 1, 4, 1, 1, 4};
    const u8 notes[6] = {2, 5, 18, 2, 5, 18};

    for (int i = 0; i < 6; i++) {
        notePlay(notes[i] & 15, notes[i] >> 4);
        VBlankIntrDelay(8 * lens[i]);
    }
}

static void
sing() noexcept {
    REG_SNDSTAT = SSTAT_ENABLE;
    REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1, 7);
    REG_SNDDSCNT = SDS_DMG100;

    REG_SND1SWEEP = SSW_OFF;
    REG_SND1CNT = SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_2;
    REG_SND1FREQ = 0;

    sos();
}

static void
drawRect(s32 left, s32 top, s32 width, s32 height, COLOR color) noexcept {
    s32 right = left + width;
    s32 bottom = top + height;
    for (s32 y = top; y < bottom; y++) {
        for (s32 x = left; x < right; x++) {
            m3_mem[y][x] = color;
        }
    }
}

static s32
abs(s32 i) noexcept {
    s32 mask = i >> 31;
    return (i ^ mask) - mask;
}

static void
drawLine(s32 x, s32 y, s32 x2, s32 y2, COLOR color) noexcept {
    s32 w = x2 - x;
    s32 h = y2 - y;
    s32 dx1 = 0, dx2 = 0, dy1 = 0, dy2 = 0;

    if (w < 0) dx1 = dx2 = -1;
    else if (w > 0) dx1 = dx2 = 1;
    if (h < 0) dy1 = -1;
    else if (h > 0) dy1 = 1;

    s32 longest = abs(w);
    s32 shortest = abs(h);
    if (shortest > longest) {
        shortest ^= longest;
        longest ^= shortest;
        shortest ^= longest;

        if (h < 0) dy2 = -1;
        else if (h > 0) dy2 = 1;
        dx2 = 0;
    }

    s32 numerator = longest >> 1;
    for (s32 i = 0; i <= longest; i++) {
        m3_mem[y][x] = color;
        numerator += shortest;
        if (numerator > longest) {
            numerator -= longest;
            x += dx1;
            y += dy1;
        } else {
            x += dx2;
            y += dy2;
        }
    }
}

static void
mode3() noexcept {
    // Set GBA rendering context to mode 3 with bitmap rendering.
    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;
}

static void
drawPattern() noexcept {
    mode3();

    int t = 0;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        int x, y;
        for (y = 0; y < SCREEN_HEIGHT; ++y) {
            for (x = 0; x < SCREEN_WIDTH; ++x) {
                int base = x & y;
                vid_mem[x + y * SCREEN_WIDTH] = RGB15_SAFE(base + t * 5, base + t * 3, base + t);
            }
        }
        ++t;
    }
#pragma clang diagnostic pop
}

static void
halt() noexcept {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        VBlankIntrWait();
    }
#pragma clang diagnostic pop
}

static s32 seed = 42;

static s32
rand_() noexcept {
    seed = 1664525 * seed + 1013904223;
    return (seed >> 16) & 0x7FFF;
}

static s32
randRange(s32 min, s32 max) noexcept {
    return rand_() % (max - min + 1) + min;
}

struct Ball {
    s32 x, y;
    s32 dx, dy;
    s32 size;
    COLOR color;
};

static void
ballStart(Ball* ball) noexcept {
    ball->dx = randRange(0, 1) ? 1 : -1;
    ball->dy = randRange(-1, 1);
}

static void
ballInit(Ball* ball, s32 x, s32 y, s32 size, COLOR color) noexcept {
    ball->x = x;
    ball->y = y;
    ball->size = size;
    ball->color = color;
    ballStart(ball);
}

static void
ballMove(Ball* ball) noexcept {
    ball->y += ball->dy;
    if (ball->y < 0 || ball->y > SCREEN_HEIGHT - ball->size) {
        ball->y = max(0, min(SCREEN_HEIGHT - ball->size, ball->y));
        ball->dy *= -1;
    }

    ball->x += ball->dx;
    if (ball->x < 0 || ball->x > SCREEN_WIDTH - ball->size) {
        ball->x = SCREEN_WIDTH >> 1;
        ball->y = SCREEN_HEIGHT >> 1;
        ballStart(ball);
    }
}

static void
ballDraw(Ball* ball) noexcept {
    drawRect(ball->x, ball->y, ball->size, ball->size, ball->color);
}

static void
ballClear(Ball* ball) noexcept {
    drawRect(ball->x, ball->y, ball->size, ball->size, CLR_BLACK);
}

static void
vsync() noexcept {
    while (REG_VCOUNT >= SCREEN_HEIGHT);
    while (REG_VCOUNT < SCREEN_HEIGHT);
}

struct Paddle {
    s32 x, y;
    s32 width, height;
    COLOR color;
};

static void
paddleInit(Paddle* paddle, s32 x, s32 y, s32 width, s32 height, COLOR color) noexcept {
    paddle->x = x;
    paddle->y = y;
    paddle->width = width;
    paddle->height = height;
    paddle->color = color;
}

static void
paddleDraw(Paddle* paddle) noexcept {
    drawRect(paddle->x, paddle->y, paddle->width, paddle->height, paddle->color);
}

static void
paddleClear(Paddle* paddle) noexcept {
    drawRect(paddle->x, paddle->y, paddle->width, paddle->height, CLR_BLACK);
}

int
main() noexcept {
    //printf_("Hello, world!");

    mode3();

    seed = 23343;

    Ball ball;
    ballInit(&ball, SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1, 10, CLR_WHITE);

    Paddle p1;
    paddleInit(&p1, 10, 60, 8, 40, CLR_BLUE);

    Paddle p2;
    paddleInit(&p2, SCREEN_WIDTH - 18, 60, 8, 40, CLR_RED);

    while (true) {
        ballClear(&ball);
        paddleClear(&p1);
        paddleClear(&p2);

        ballMove(&ball);

        ballDraw(&ball);
        paddleDraw(&p1);
        paddleDraw(&p2);

        drawLine(10, 4, 230, 4, CLR_WHITE);
        drawLine(230, 156, 10, 156, CLR_WHITE);

        vsync();
    }

    //REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

    /*
    mode3();

    COLOR salmon = RGB15(31, 5, 12);
    drawRect(20, 20, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 40, salmon);

    COLOR green = RGB15(1, 24, 16);
    drawLine(20 - 1, 20 - 1, 0 - 1, 0 - 1, green); // Top-left.
    drawLine(220, 20 - 1, 240, 0 - 1, green); // Top-right.
    drawLine(20 - 1, 140, 0 - 1, 160, green); // Bottom-left.
    drawLine(220, 140, 240, 160, green); // Bottom-right.
    */

    /*
    irq_init(nullptr);
    irq_add(II_VBLANK, nullptr);

    //sing();

    drawPattern();

    halt();
    */
}
