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
