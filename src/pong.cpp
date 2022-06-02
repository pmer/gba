#include "pong.h"

#include "gfx.h"
#include "math.h"

#include "hw/input.h"
#include "hw/video.h"

struct Ball {
    i32 x, y;
    i32 dx, dy;
    i32 size;
    Color color;
};

static void
ballStart(Ball* ball) noexcept {
    ball->dx = randRange(0, 1) ? 1 : -1;
    ball->dy = randRange(-1, 1);
}

static void
ballInit(Ball* ball, i32 x, i32 y, i32 size, Color color) noexcept {
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

struct Paddle {
    i32 x, y;
    i32 width, height;
    Color color;
};

static void
paddleInit(Paddle* paddle, i32 x, i32 y, i32 width, i32 height, Color color) noexcept {
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

static void
drawNet() noexcept {
    const int W = 2;
    const int H = 4;
    for (int i = H >> 1; i < SCREEN_HEIGHT - H; i += 2 * H) {
        drawRect((SCREEN_WIDTH >> 1) - (W >> 1), i, W, H, CLR_WHITE);
    }
}

void
pong() noexcept {
    randSeed(23343);

    Ball ball;
    ballInit(&ball, SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1, 4, CLR_WHITE);

    Paddle p1, p2;
    paddleInit(&p1, 10, 60, 4, 14, CLR_WHITE);
    paddleInit(&p2, SCREEN_WIDTH - 18, 60, 4, 14, CLR_WHITE);

    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

    while (true) {
        ballClear(&ball);
        paddleClear(&p1);
        paddleClear(&p2);

        ballMove(&ball);

        if (KEY_IS_DOWN(KEY_UP)) {
            p1.y -= 1;
            p1.y = max(0, p1.y);
        }
        if (KEY_IS_DOWN(KEY_DOWN)) {
            p1.y += 1;
            p1.y = min(p1.y, SCREEN_HEIGHT - p1.height);
        }

        ballDraw(&ball);
        paddleDraw(&p1);
        paddleDraw(&p2);

        drawNet();

        vsync();
    }
}
