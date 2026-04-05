#include "usart.h"

// ****** USART INIT ******

void ubegin(const uint16_t baud) {
    uint16_t usart_speed = (uint16_t)(F_CPU / (16UL * baud) - 1UL);
    UBRR0H = (uint8_t)(usart_speed >> 8);
    UBRR0L = (uint8_t)(usart_speed);

    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

// ****** USART PRINT STR ******

void uput_ch(const char ch) {
    if (ch == '\n') {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = '\r';
    }
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 =  ch;
}

void uput_str(const char *str) {
    char ch;
    while ((ch = *str)) {
        uput_ch(ch);
        str++;
    }
}

// ****** USART SCAN STR ******

//TODO: this
// char uget_ch(void) {
//     while (!(UCSR0A & (1 << RXC0)));
//     return UDR0;
// }

// ****** USART PRINT DEC ******

void uprint_dec(const uint16_t num) {
    for (uint8_t deg = 4; deg > 0; deg--) {
        uint16_t divnum = divu10d(num, deg - 1);
        uint16_t rem = remu10(divnum);

        if (divnum == 0 && (deg - 1) != 0) continue;
        else uput_ch(('0' + rem));
    }
}

// ****** USART PRINT BIN ******

void uprint_bin(const uint16_t num) {
    uint8_t deg = num > 0xff ? 16 : 8;

    uput_str("0b");
    for (; deg > 0; deg--) {
        uint16_t divnum = divu2d(num, deg - 1);
        uint16_t rem = remu2(divnum);

        uput_ch(('0' + rem));
        if (deg == 9) uput_ch(' ');
    }
}

// ****** USART PRINT HEX ******

static inline uint16_t hex_mask(const uint8_t num) {
    return num > 9 ? ('A' + (num - 10)) : ('0' + num);
}

void uprint_hex(const uint16_t num) {
    uint8_t deg = num > 0xff ? 4 : 2;

    uput_str("0x");
    for (; deg > 0; deg--) {
        uint16_t divnum = divu16d(num, deg - 1);
        uint16_t rem = remu16(divnum);
        char hex_num = hex_mask(rem);

        uput_ch(hex_num);
    }
}

// ****** USART PRINT ******

void uprint_write(const char *fmt, uint8_t len, uint16_t *args) {
    uint8_t index = 0;
    char ch0, ch1;

    while ((ch0 = *fmt)) {
        fmt++;
    
        if (ch0 == '%' && index < len) {
            ch1 = *fmt;
            fmt++;
        
            switch (ch1) {
            case 'd': case 'D':
                uprint_dec(args[index]);
                break;
            case 'b': case 'B':
                uprint_bin(args[index]);
                break;
            case 'x': case 'X':
                uprint_hex(args[index]);
                break;
            default:
                uput_ch(ch0);
                uput_ch(ch1);
                break;
            }
        
            index++;
        } else uput_ch(ch0);
    }
}