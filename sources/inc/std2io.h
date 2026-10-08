#ifndef STD2IO_H
#define STD2IO_H

static inline long _strlen(const char *s) {
    long len = 0;
    while (s && s[len]) len++;
    return len;
}

static inline long _append_str(char *buf, long pos, const char *s) {
    int i = 0;
    while (s[i] != '\0' && pos < 4000) {
        buf[pos++] = s[i++];
    }
    return pos;
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
    while (tbuf[i] != '\0') {
        buf[pos++] = tbuf[i++];
    }
    return pos;
}

static inline void _write_flush(const char *buf, long len) {
    if (len <= 0) return;
    register long x0 asm("x0") = 1;
    register const char *x1 asm("x1") = buf;
    register long x2 asm("x2") = len;
    register long x8 asm("x8") = 64;
    __sync_synchronize();
    asm volatile("svc #0" : "+r"(x0) : "r"(x1), "r"(x2), "r"(x8) : "memory", "cc");
    __sync_synchronize();
}

#define _AUTO_APPEND(buf, pos, val) _Generic((val), \
    char*: _append_str(buf, pos, (char*)(val)), \
    const char*: _append_str(buf, pos, (const char*)(val)), \
    default: _append_num(buf, pos, (long)(val)) \
)

#define _WR_1(b, p, a)          p = _AUTO_APPEND(b, p, a);
#define _WR_2(b, p, a, b_)      p = _AUTO_APPEND(b, p, a); p = _AUTO_APPEND(b, p, b_);
#define _WR_3(b, p, a, b_, c)   p = _AUTO_APPEND(b, p, a); p = _AUTO_APPEND(b, p, b_); p = _AUTO_APPEND(b, p, c);
#define _WR_4(b, p, a, b_, c, d) p = _AUTO_APPEND(b, p, a); p = _AUTO_APPEND(b, p, b_); p = _AUTO_APPEND(b, p, c); p = _AUTO_APPEND(b, p, d);
#define _WR_5(b, p, a, b_, c, d, e) p = _AUTO_APPEND(b, p, a); p = _AUTO_APPEND(b, p, b_); p = _AUTO_APPEND(b, p, c); p = _AUTO_APPEND(b, p, d); p = _AUTO_APPEND(b, p, e);

#define _GET_WR_M(_1,_2,_3,_4,_5,NAME,...) NAME

#define write(...) do { \
    char out_buf[4096]; \
    long pos = 0; \
    _GET_WR_M(__VA_ARGS__, _WR_5, _WR_4, _WR_3, _WR_2, _WR_1)(out_buf, pos, __VA_ARGS__) \
    _write_flush(out_buf, pos); \
} while(0)

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
