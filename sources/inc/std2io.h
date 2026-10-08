#ifndef STD2IO_H
#define STD2IO_H

static inline long _strlen(const char *s) {
    long len = 0;
    while (s && s[len]) len++;
    return len;
}

static inline long _append_num(char *buf, long pos, long num) {
    char tbuf[32];
    int i = 30;
    tbuf[31] = '\0';
    long n = num;
    int neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    } else if (n == 0) {
        tbuf[--i] = '0';
    }
    while (n > 0) {
        tbuf[--i] = '0' + (n % 10);
        n /= 10;
    }
    if (neg) {
        tbuf[--i] = '-';
    }
    while (tbuf[i] != '\0' && pos < 4000) {
        buf[pos++] = tbuf[i++];
    }
    return pos;
}

static inline void _write_format(const char *fmt, ...) {
    if (!fmt || fmt[0] == '\0') return;
    char out_buf[4096];
    long pos = 0;
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    for (int i = 0; fmt[i] != '\0' && pos < 4000; i++) {
        if (fmt[i] == '<') {
            while (fmt[i] != '\0' && fmt[i] != '>') {
                i++;
            }
            long val = __builtin_va_arg(args, long);
            pos = _append_num(out_buf, pos, val);
        } else {
            out_buf[pos++] = fmt[i];
        }
    }
    __builtin_va_end(args);
    if (pos > 0) {
        register long x0 asm("x0") = 1;
        register const char *x1 asm("x1") = out_buf;
        register long x2 asm("x2") = pos;
        register long x8 asm("x8") = 64;
        __sync_synchronize();
        asm volatile("svc #0" : "+r"(x0) : "r"(x1), "r"(x2), "r"(x8) : "memory", "cc");
        __sync_synchronize();
    }
}

#define write(...) _write_format(__VA_ARGS__)

static inline long _read_internal(char *buf, long max_len) {
    if (max_len <= 0) return 0;
    register long x0 asm("x0") = 0;
    register char *x1 asm("x1") = buf;
    register long x2 asm("x2") = max_len - 1;
    register long x8 asm("x8") = 63;
    __sync_synchronize();
    asm volatile("svc #0" : "+r"(x0) : "r"(x1), "r"(x2), "r"(x8) : "memory", "cc");
    __sync_synchronize();
    if (x0 >= 0) buf[x0] = '\0';
    return x0;
}

#define _GET_READ_MACRO(_1, _2, NAME, ...) NAME
#define read(...) _GET_READ_MACRO(__VA_ARGS__, _read_explicit, _read_auto)(__VA_ARGS__)
#define _read_auto(buffer) _read_internal(buffer, sizeof(buffer))
#define _read_explicit(buffer, size) _read_internal(buffer, size)

#endif
