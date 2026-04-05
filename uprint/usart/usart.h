#ifndef USARTPRINT_H_INCLUDED
#define USARTPRINT_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>
#include <bitmath.h>

#ifdef __cplusplus
extern "C" {
#endif

// ****** FUNCTIONS ******

void ubegin(const uint16_t baud);

void uput_ch(const char ch);
void uput_str(const char *str);

void uprint_dec(const uint16_t num);
void uprint_bin(const uint16_t num);
void uprint_hex(const uint16_t num);

void uprint_write(const char *fmt, uint8_t len, uint16_t *args);

// ****** ARGS_COUNT ******

#define ARGS_COUNT_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) N
#define ARGS_COUNT(...) ARGS_COUNT_(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// ****** UPRINT 0 ARGS ******

#define uprint0(fmt) uprint_write(fmt, 0, NULL)

#define uprintln0(fmt)            \
    do {                          \
        uprint0(fmt);             \
        uput_ch('\n');            \
    } while(0)


// ****** UPRINT NORMAL ******
    
#define uprint(fmt, ...)                                                                    \
    do {                                                                                    \
        _Static_assert(sizeof(#__VA_ARGS__) > 1, "uprint requires at least one argument."); \
        uprint_write(fmt, ARGS_COUNT(__VA_ARGS__), (uint16_t[]){__VA_ARGS__});              \
    } while(0)

#define uprintln(fmt, ...)        \
    do {                          \
        uprint(fmt, __VA_ARGS__); \
        uput_ch('\n');            \
    } while(0)

#ifdef __cplusplus
}
#endif

#endif //USARTPRINT_H_INCLUDED