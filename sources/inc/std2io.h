#ifndef STD2IO_H
#define STD2IO_H

static inline long _strlen(const char *s) {
    long len = 0;
    while (s[len]) len++;
    return len;
}

static inline long write(const char *s) {
    long len = _strlen(s);

    register long x0 asm("x0") = 1;
    register const char *x1 asm("x1") = s;
    register long x2 asm("x2") = len;
    register long x8 asm("x8") = 64;

    __sync_synchronize();
    asm volatile(
        "svc #0"
        : "+r"(x0)
        : "r"(x1), "r"(x2), "r"(x8)
        : "memory", "cc"
    );
    __sync_synchronize();
    return x0;
}

static inline long _read_internal(char *buf, long max_len) {
    if (max_len <= 0) return 0;

    register long x0 asm("x0") = 0;
    register char *x1 asm("x1") = buf;
    register long x2 asm("x2") = max_len - 1;
    register long x8 asm("x8") = 63;

    __sync_synchronize();
    asm volatile(
        "svc #0"
        : "+r"(x0)
        : "r"(x1), "r"(x2), "r"(x8)
        : "memory", "cc"
    );
    __sync_synchronize();

    if (x0 >= 0) {
        buf[x0] = '\0';
    }
    return x0;
}

#define read(buffer) _read_internal(buffer, sizeof(buffer))

#endif
