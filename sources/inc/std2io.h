#ifndef STD2IO_H
#define STD2IO_H

static char *__std2_global_buf = 0;

static inline void _write_format(const char *fmt) {
    if (!fmt) return;

    char out_buf[4000000];
    long pos = 0;
    char *gbuf = __std2_global_buf;

    asm volatile (
        "mov x3, #0\n\t"
        "mov x4, %[pos]\n\t"

    "1:\n\t"
        "ldrb w5, [%[fmt], x3]\n\t"
        "cbz w5, 5f\n\t"
        "cmp x4, #4000\n\t"
        "bge 5f\n\t"

        "cmp w5, #'<'\n\t"
        "bne 4f\n\t"

    "2:\n\t"
        "add x3, x3, #1\n\t"
        "ldrb w5, [%[fmt], x3]\n\t"
        "cbz w5, 5f\n\t"
        "cmp w5, #'>'\n\t"
        "bne 2b\n\t"
        "add x3, x3, #1\n\t"

        "cbz %[gbuf], 1b\n\t"
        "mov x6, #0\n\t"

    "3:\n\t"
        "ldrb w7, [%[gbuf], x6]\n\t"
        "cbz w7, 1b\n\t"
        "cmp x4, #4000\n\t"
        "bge 5f\n\t"
        "strb w7, [%[out_buf], x4]\n\t"
        "add x4, x4, #1\n\t"
        "add x6, x6, #1\n\t"
        "b 3b\n\t"

    "4:\n\t"
        "strb w5, [%[out_buf], x4]\n\t"
        "add x4, x4, #1\n\t"
        "add x3, x3, #1\n\t"
        "b 1b\n\t"

    "5:\n\t"
        "mov %[pos], x4\n\t"
        : [pos] "+r" (pos)
        : [fmt] "r" (fmt), [gbuf] "r" (gbuf), [out_buf] "r" (out_buf)
        : "x3", "x4", "x5", "x6", "x7", "memory", "cc"
    );

    if (pos > 0) {
        register long x0 asm("x0") = 1;
        register const char *x1 asm("x1") = out_buf;
        register long x2 asm("x2") = pos;
        register long x8 asm("x8") = 64;
        __sync_synchronize();
        asm volatile (
            "svc #0"
            : "+r" (x0)
            : "r" (x1), "r" (x2), "r" (x8)
            : "memory", "cc"
        );
        __sync_synchronize();
    }
}

#define write(fmt) _write_format(fmt)

static inline long _read_internal(char *buf, long max_len) {
    if (max_len <= 0) return 0;
    __std2_global_buf = buf;

    register long x0 asm("x0") = 0;
    register char *x1 asm("x1") = buf;
    register long x2 asm("x2") = max_len - 1;
    register long x8 asm("x8") = 63;

    __sync_synchronize();
    asm volatile (
        "svc #0"
        : "+r" (x0)
        : "r" (x1), "r" (x2), "r" (x8)
        : "memory", "cc"
    );
    __sync_synchronize();

    if (x0 >= 0) {
        asm volatile (
            "strb wzr, [%0, %1]"
            :
            : "r" (buf), "r" (x0)
            : "memory"
        );
    }
    return x0;
}

#define _GET_READ_MACRO(_1, _2, NAME, ...) NAME
#define read(...) _GET_READ_MACRO(__VA_ARGS__, _read_explicit, _read_auto)(__VA_ARGS__)
#define _read_auto(buffer) _read_internal(buffer, sizeof(buffer))
#define _read_explicit(buffer, size) _read_internal(buffer, size)

#endif
