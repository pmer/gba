#include "pong.h"

#include "gfx.h"
#include "math.h"
#include "print.h"

#include "hw/input.h"
#include "hw/video.h"

#define B_W 4
#define B_H B_W
#define P_W 4
#define P_H 14

static bool
areIntersecting(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) noexcept {
    int l1 = x1;
    int l2 = x2;
    int r1 = x1 + w1;
    int r2 = x2 + w2;
    int t1 = y1;
    int t2 = y2;
    int b1 = y1 + h1;
    int b2 = y2 + h2;
    return l1 < r2 && l2 < r1 && t1 < b2 && t2 < b1;
}

struct Ball {
    i32 x, y;
    i32 dx, dy;
    i32 size;
    Color color;
};

struct Paddle {
    i32 x, y;
    i32 width, height;
    Color color;
};

static void
ballStart(Ball *ball) noexcept {
    ball->dx = randRange(0, 1) ? 1 : -1;
    ball->dy = randRange(-1, 1);
}

static void
ballInit(Ball *ball, i32 x, i32 y, i32 size, Color color) noexcept {
    ball->x = x;
    ball->y = y;
    ball->size = size;
    ball->color = color;
    ballStart(ball);
}

static void
ballMove(Ball *ball) noexcept {
    ball->y += ball->dy;
    if (ball->y < 0 || ball->y > SCREEN_HEIGHT - ball->size) {
        ball->y = max(0, min(SCREEN_HEIGHT - ball->size, ball->y));
        ball->dy *= -1;
    }

    ball->x += ball->dx;
    if (ball->x < 0 || ball->x > SCREEN_WIDTH - ball->size) {
        ball->x = (SCREEN_WIDTH >> 1) - (ball->size >> 1);
        ball->y = (SCREEN_HEIGHT >> 1) - (ball->size >> 1);
        ballStart(ball);
    }
}

static void
ballIntersect(Ball *ball, Paddle *p) noexcept {
    if (areIntersecting(ball->x, ball->y, B_W, B_H, p->x, p->y, P_W, P_H)) {
        ball->dx = -ball->dx;
        int byc = ball->y + (B_H >> 1);
        int pyc = p->y + (P_H >> 1);
        int dyc = byc - pyc; // [? to ?]
        int ycSpan = (B_W >> 1) + (P_H >> 1); // 9
        ball->dy = dyc >> 1; // [-4 to 4]
    }
}

static void
ballDraw(Ball *ball) noexcept {
    drawRect(ball->x, ball->y, ball->size, ball->size, ball->color);
}

static void
ballClear(Ball *ball) noexcept {
    drawRect(ball->x, ball->y, ball->size, ball->size, CLR_BLACK);
}

static void
paddleInit(Paddle *p, i32 x, i32 y, i32 width, i32 height, Color color) noexcept {
    p->x = x;
    p->y = y;
    p->width = width;
    p->height = height;
    p->color = color;
}

static void
paddleAI(Paddle *p, Ball *b) noexcept {
    int byc = b->y + (B_H >> 1);
    int pyc = p->y + (P_H >> 1);
    if (byc < pyc) {
        p->y -= 1;
        p->y = max(0, p->y);
    }
    else if (byc > pyc) {
        p->y += 1;
        p->y = min(p->y, SCREEN_HEIGHT - P_H);
    }
}

static void
paddleDraw(Paddle *p) noexcept {
    drawRect(p->x, p->y, P_W, P_H, p->color);
}

static void
paddleClear(Paddle *p) noexcept {
    drawRect(p->x, p->y, P_W, P_H, CLR_BLACK);
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
    randSeed(15);

    drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLR_BLACK);

    Ball ball;
    ballInit(&ball, SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1, B_W, CLR_WHITE);

    Paddle p1, p2;
    paddleInit(&p1, 40 - (P_W >> 1), (SCREEN_HEIGHT >> 1) - (P_H >> 1), P_W, P_H, CLR_WHITE);
    paddleInit(&p2, SCREEN_WIDTH - 40 - (P_W >> 1), (SCREEN_HEIGHT >> 1) - (P_H >> 1), P_W, P_H, CLR_WHITE);

    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

    while (true) {
        ballClear(&ball);
        paddleClear(&p1);
        paddleClear(&p2);

        if (KEY_IS_DOWN(KEY_UP)) {
            p1.y -= 1;
            p1.y = max(0, p1.y);
        }
        if (KEY_IS_DOWN(KEY_DOWN)) {
            p1.y += 1;
            p1.y = min(p1.y, SCREEN_HEIGHT - p1.height);
        }

        ballMove(&ball);
        ballIntersect(&ball, &p1);
        ballIntersect(&ball, &p2);
        paddleAI(&p2, &ball);

        ballDraw(&ball);
        paddleDraw(&p1);
        paddleDraw(&p2);

        drawNet();

        vsync();
    }
}
