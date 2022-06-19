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

#define AI 0

#define Y_RES 8

static int score1 = 0;
static int score2 = 0;

template<int Bits>
class Frac {
 public:
    int value;

    static inline Frac
    fromWhole(int value) noexcept {
        return {value << Bits};
    }

    static inline Frac
    fromFrac(int value) noexcept {
        return {value};
    }

    inline int
    get() noexcept {
        return value >> Bits;
    }

    inline void operator+=(Frac other) noexcept { value += other.value; }

    inline void operator*=(int other) noexcept { value *= other; }

    inline Frac operator+(Frac other) noexcept { return {value + other.value}; }

    inline Frac operator+(int other) noexcept { return {value + (other << Bits)}; }

    inline Frac operator-(Frac other) noexcept { return {value - other.value}; }

    inline Frac operator-(int other) noexcept { return {value - (other << Bits)}; }

    inline Frac operator>>(int count) noexcept { return {value >> count}; }

    inline Frac operator<<(int count) noexcept { return {value << count}; }

    inline bool operator<(int other) noexcept { return value >> Bits < other; }

    inline bool operator>(int other) noexcept { return value >> Bits > other; }

    inline bool operator<(Frac other) noexcept { return value < other.value; }

    inline bool operator>(Frac other) noexcept { return value > other.value; }
};

using Y = Frac<Y_RES>;

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
    i32 x;
    Y y;
    i32 dx;
    Y dy;
    i32 size;
    Color color;
};

struct Paddle {
    i32 x;
    Y y;
    i32 width, height;
    Color color;
};

static void
ballStart(Ball *ball) noexcept {
    ball->dx = randRange(0, 1) ? 1 : -1;
    // -1 because otherwise it's hard to hit.
    int yRange = 1 << (Y_RES-1);
    ball->dy = Y::fromFrac(randRange(-yRange, yRange));
}

static void
ballInit(Ball *ball, i32 x, Frac<Y_RES> y, i32 size, Color color) noexcept {
    ball->x = x;
    ball->y = y;
    ball->size = size;
    ball->color = color;
    ballStart(ball);
}

static void
ballMove(Ball *ball) noexcept {
    Y zero = Y::fromWhole(0);
    Y screenHeight = Y::fromWhole(SCREEN_HEIGHT);
    Y ballSize = Y::fromWhole(B_H);

    ball->y += ball->dy;
    if (ball->y < zero || ball->y > screenHeight - ballSize) {
        ball->y = max(zero, min(screenHeight - ballSize, ball->y));
        ball->dy *= -1;
    }

    ball->x += ball->dx;
    if (ball->x < 0 || ball->x > SCREEN_WIDTH - ball->size) {
        if (ball->x < 0) {
            score2++;
        } else {
            score1++;
            randConsume(1);
        }
        int score = score1 + score2;
        int xRange = max(3 * score, 10);
        int yRange = max(3 * score, 15) << Y_RES;
        ball->x = (SCREEN_WIDTH >> 1) - (ball->size >> 1) + randRange(-xRange, xRange);
        ball->y = (screenHeight >> 1) - (ballSize >> 1) + Y::fromFrac(randRange(-yRange, yRange));
        ballStart(ball);
    }
}

static void
ballIntersect(Ball *ball, Paddle *p) noexcept {
    if (areIntersecting(ball->x, ball->y.get(), B_W, B_H, p->x, p->y.get(), P_W, P_H)) {
        ball->dx = -ball->dx;
        Y byc = ball->y + (B_H >> 1);
        Y pyc = p->y + (P_H >> 1);
        Y dyc = byc - pyc; // [? to ?]
        //Y ycSpan = Y::fromWhole((B_W >> 1) + (P_H >> 1)); // 9
        ball->dy = dyc >> 1; // [-4 to 4]
        ball->dx += randRange(-1, 1);
        ballMove(ball);
    }
}

static void
ballDraw(Ball *ball) noexcept {
    drawRect(ball->x, ball->y.get(), ball->size, ball->size, ball->color);
}

static void
ballClear(Ball *ball) noexcept {
    drawRect(ball->x, ball->y.get(), ball->size, ball->size, CLR_BLACK);
}

static void
paddleInit(Paddle *p, i32 x, Y y, i32 width, i32 height, Color color) noexcept {
    p->x = x;
    p->y = y;
    p->width = width;
    p->height = height;
    p->color = color;
}

static void
paddleMove(Paddle *p, Y dy) noexcept {
    Y zero = Y::fromWhole(0);
    Y screenHeight = Y::fromWhole(SCREEN_HEIGHT);
    Y pH = Y::fromWhole(P_H);

    p->y += dy;
    p->y = max(zero, min(p->y, screenHeight - pH));
}

static void
paddleAI(Paddle *p, Ball *b) noexcept {
    Y negOne = Y::fromWhole(-1);
    Y one = Y::fromWhole(1);
    Y bH = Y::fromWhole(B_H);
    Y pH = Y::fromWhole(P_H);

    Y byc = b->y + (bH >> 1);
    Y pyc = p->y + (pH >> 1);
    if (byc < pyc) {
        paddleMove(p, negOne);
    } else if (byc > pyc) {
        paddleMove(p, one);
    }
}

static void
paddleDraw(Paddle *p) noexcept {
    drawRect(p->x, p->y.get(), P_W, P_H, p->color);
}

static void
paddleClear(Paddle *p) noexcept {
    drawRect(p->x, p->y.get(), P_W, P_H, CLR_BLACK);
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
    Y negOne = Y::fromWhole(-1);
    Y one = Y::fromWhole(1);
    Y screenHeight = Y::fromWhole(SCREEN_HEIGHT);

    randSeed(15);

    drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLR_BLACK);

    Ball ball;
    ballInit(&ball, SCREEN_WIDTH >> 1, screenHeight >> 1, B_W, CLR_WHITE);

    Paddle p1, p2;
    paddleInit(&p1, 40 - (P_W >> 1), (screenHeight >> 1) - (P_H >> 1), P_W, P_H, CLR_WHITE);
    paddleInit(&p2, SCREEN_WIDTH - 40 - (P_W >> 1), (screenHeight >> 1) - (P_H >> 1), P_W, P_H, CLR_WHITE);

    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

    while (true) {
        ballClear(&ball);
        paddleClear(&p1);
        paddleClear(&p2);

        if (KEY_IS_DOWN(KEY_UP)) {
            paddleMove(&p1, negOne);
            randConsume(1);
        }
        if (KEY_IS_DOWN(KEY_DOWN)) {
            paddleMove(&p1, one);
            randConsume(2);
        }

#if AI
        paddleAI(&p2, &ball);
#else
        if (KEY_IS_DOWN(KEY_B)) {
            paddleMove(&p2, negOne);
            randConsume(3);
        }
        if (KEY_IS_DOWN(KEY_A)) {
            paddleMove(&p2, one);
            randConsume(4);
        }
#endif

        ballMove(&ball);
        ballIntersect(&ball, &p1);
        ballIntersect(&ball, &p2);

        ballDraw(&ball);
        paddleDraw(&p1);
        paddleDraw(&p2);

        drawNet();

        vsync();
    }
}
