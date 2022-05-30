#include "print.h"

#include <stdarg.h>
#include <string.h>
#include <tonc_types.h>

#define REG_DEBUG_ENABLE (vu16*)0x4FFF780
#define REG_DEBUG_FLAGS (vu16*)0x4FFF700
#define REG_DEBUG_STRING (char*)0x4FFF600

#define MGBA_LOG_FATAL 0
#define MGBA_LOG_ERROR 1
#define MGBA_LOG_WARN 2
#define MGBA_LOG_INFO 3
#define MGBA_LOG_DEBUG 4

#define STATUS_UNINITIALIZED 0
#define STATUS_OKAY 1
#define STATUS_NOT_MGBA 2

static int status = STATUS_UNINITIALIZED;

static bool
isMgba() noexcept {
    if (status == STATUS_UNINITIALIZED) {
        *REG_DEBUG_ENABLE = 0xC0DE;
        status = *REG_DEBUG_ENABLE == 0x1DEA ? STATUS_OKAY : STATUS_NOT_MGBA;
    }
    return status == STATUS_OKAY;
}

static char*
ntoa(char* buf, unsigned long i) noexcept {
    char digits[10];
    char* p = digits;

    do {
        *p++ = i % 10 + '0';
        i /= 10;
    } while (i > 0);

    do {
        *buf++ = *--p;
    } while (p != digits);

    return buf;
}

static char*
ntoh(char* buf, size_t i) noexcept {
    char hex[16];
    char* p = hex;

    do {
        size_t nibble = i & 15;
        *p++ = nibble + (nibble > 9 ? 'a' : '0');
        i >>= 4;
    } while (i > 0);

    do {
        *buf++ = *--p;
    } while (p != hex);

    return buf;
}

static void
vsprintf_(char* buf, const char* format, va_list va) noexcept {
    char c;
    while ((c = *format++)) {
        if (c != '%') {
            *buf++ = c;
            continue;
        }
        c = *format++;
        switch (c) {
            case 'c':
                *buf++ = va_arg(va, int);
                break;
            case 'd': {
                int i = va_arg(va, int);
                if (i < 0) {
                    i = -i;
                    *buf++ = '-';
                }
                buf = ntoa(buf, i);
                break;
            }
            case 'p':
                buf[0] = '0';
                buf[1] = 'x';
                buf = ntoh(buf + 2, (size_t)va_arg(va, char*));
                break;
            case 's': {
                const char* p = va_arg(va, char*);
                size_t len = strlen(p);
                memcpy(buf, p, len);
                buf += len;
                break;
            }
            case 'u':
                buf = ntoa(buf, va_arg(va, unsigned int));
                break;
            default: {
                *buf++ = c;
                break;
            }
        }
    }
    *buf = 0;
}

void
printf_(const char* format, ...) noexcept {
    if (!isMgba()) {
        return;
    }
    va_list va;
    va_start(va, format);
    vsprintf_(REG_DEBUG_STRING, format, va);
    va_end(va);
    *REG_DEBUG_FLAGS = MGBA_LOG_WARN | 0x100;
}
