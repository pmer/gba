#include "pong.h"

#include "gfx.h"
#include "math.h"
#include "reg.h"
#include "video.h"

extern "C" {
#include <tonc_math.h>
#include <tonc_types.h>
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
    if (ball->y < 0 || ball->y > SCREEN_HEIGHT_ - ball->size) {
        ball->y = max(0, min(SCREEN_HEIGHT_ - ball->size, ball->y));
        ball->dy *= -1;
    }

    ball->x += ball->dx;
    if (ball->x < 0 || ball->x > SCREEN_WIDTH_ - ball->size) {
        ball->x = SCREEN_WIDTH_ >> 1;
        ball->y = SCREEN_HEIGHT_ >> 1;
        ballStart(ball);
    }
}

static void
ballDraw(Ball* ball) noexcept {
    drawRect(ball->x, ball->y, ball->size, ball->size, ball->color);
}

static void
ballClear(Ball* ball) noexcept {
    drawRect(ball->x, ball->y, ball->size, ball->size, CLR_BLACK_);
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
    drawRect(paddle->x, paddle->y, paddle->width, paddle->height, CLR_BLACK_);
}

void
pong() noexcept {
    randSeed(23343);

    Ball ball;
    ballInit(&ball, SCREEN_WIDTH_ >> 1, SCREEN_HEIGHT_ >> 1, 10, CLR_WHITE_);

    Paddle p1, p2;
    paddleInit(&p1, 10, 60, 8, 40, CLR_BLUE_);
    paddleInit(&p2, SCREEN_WIDTH_ - 18, 60, 8, 40, CLR_RED_);

    mode3();

    while (true) {
        ballClear(&ball);
        paddleClear(&p1);
        paddleClear(&p2);

        ballMove(&ball);

        ballDraw(&ball);
        paddleDraw(&p1);
        paddleDraw(&p2);

        drawLine(10, 4, 230, 4, CLR_WHITE_);
        drawLine(230, 156, 10, 156, CLR_WHITE_);

        vsync();
    }
}
