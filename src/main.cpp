#include "print.h"

/*
#if defined(__thumb__)
#define BiosSystemCall(Number) __asm ("SWI "#Number"\n" ::: "r0", "r1", "r2", "r3")
#else
#define	BiosSystemCall(Number) __asm ("SWI "#Number" << 16\n" ::: "r0", "r1", "r2", "r3")
#endif

// Don't let the compiler optimise seemingly empty bios call statements away
#pragma GCC push_options
#pragma GCC optimize ("O0")

// Will crash on actual hardware
inline void AGBPrint(const char *message) {
    BiosSystemCall(0xFF);
}

#pragma GCC pop_options
*/


int main() {
    printf("sizeof(int): %d", sizeof(int));
    printf("Setting mode to 3 at %p", 0x04000000);

    // Set GBA rendering context to MODE 3 Bitmap Rendering.
    *(unsigned int *) 0x04000000 = 0x0403;

    int t = 0;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        int x, y;
        for (y = 0; y < 160; ++y) {
            for (x = 0; x < 240; ++x) {
                ((unsigned short *) 0x06000000)[x + y * 240] = ((((x & y) + t) & 0x1F) << 10) |
                                                               ((((x & y) + t * 3) & 0x1F) << 5) |
                                                               ((((x & y) + t * 5) & 0x1F) << 0);
            }
        }
        ++t;
    }
#pragma clang diagnostic pop
    return 0;
}
