#ifndef BITMATH_H_INCLUDED
#define BITMATH_H_INCLUDED

#include <avr/io.h>

// ****** DIV && REM 10 ******

static inline uint16_t divu10(uint16_t num) {
    uint16_t quot, rem;

    quot = (num >> 1) + (num >> 2);
    quot += (quot >> 4);
    quot += (quot >> 8);
    quot >>= 3;
    rem = num - (((quot << 2) + quot) << 1);

    return quot + (rem > 9);
}

static inline uint16_t remu10(uint16_t num) {
    uint16_t quot, rem;

    quot = (num >> 1) + (num >> 2);
    quot += (quot >> 4);
    quot += (quot >> 8);
    quot >>= 3;
    rem = num - (((quot << 2) + quot) << 1);

    return rem > 9 ? rem - 10 : rem;
}

static inline uint16_t divu10d(uint16_t num, uint8_t deg) {
    deg = deg > 4 ? 4 : deg;

    for (uint8_t i = 0; i < deg; i++) {
        num = divu10(num);
    }
    return num;
}

static inline uint16_t remu10d(uint16_t num, uint8_t deg) {
    uint16_t rem = 0;
    uint16_t mul = 1;

    deg = deg > 4 ? 4 : deg;

    for (uint8_t i = 0; i < deg; i++) {
        rem += remu10(num) * mul;
        mul *= 10;
        num = divu10(num);
    }
    return rem;
}

// ****** DIV && REM 2 ******

static inline uint16_t divu2(uint16_t num) {
    return num >> 1;
}

static inline uint16_t remu2(uint16_t num) {
    return num & 0x01;
}

static inline uint16_t divu2d(uint16_t num, uint8_t deg) {
    deg = deg > 16 ? 16 : deg;
    return num >> deg;
}

static inline uint16_t remu2d(uint16_t num, uint8_t deg) {
    uint16_t mask = 0x00;

    deg = deg > 16 ? 16 : deg;

    if (deg == 1)       mask = 0x01;
    else if (deg <= 4)  mask = 0x0f;
    else if (deg <= 8)  mask = 0xff;
    else if (deg <= 12) mask = 0x0fff;
    else if (deg <= 16) mask = 0xffff;

    return num & mask;
}

// ****** DIV && REM 16 ******

static inline uint16_t divu16(uint16_t num) {
    return num >> 4;
}

static inline uint16_t remu16(uint16_t num) {
    return num & 0x0f;
}

static inline uint16_t divu16d(uint16_t num, uint8_t deg) {
    deg = deg > 4 ? 4 : deg;
    return num >> (4 * deg);
}

static inline uint16_t remu16d(uint16_t num, uint8_t deg) {
    uint16_t mask = 0x00;

    deg = deg > 4 ? 4 : deg;

    switch (deg) {
    case 1: mask = 0x0f;   break;
    case 2: mask = 0xff;   break;
    case 3: mask = 0x0fff; break;
    case 4: mask = 0xffff; break;
    }

    return num & mask;
}

#endif // BITMATH_H_INCLUDED