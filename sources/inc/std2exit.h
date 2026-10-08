#ifndef STD2EXIT_H
#define STD2EXIT_H

static inline void _exit_raw(long status) __attribute__((noreturn, always_inline));

static inline void _exit_raw(long status) {
    register long x0 asm("x0") = status;
    register long x8 asm("x8") = 93;
    asm volatile (
        "svc #0\n\t"
        "brk #0"
        :
        : "r" (x0), "r" (x8)
        : "memory"
    );
}

#define exit _exit_raw(0)

#endif
