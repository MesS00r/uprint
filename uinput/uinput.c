#include "uinput.hpp"

// ****** USART BUFFER STRUCT ******

struct sys_buffer {
    char buffer[BUFFER_SIZE];
    uint8_t first, last;
};



sys_buffer* buffer_init(void) {
    static struct sys_buffer global = {
        .buffer = {0},
        .first = 0, .last = 0
    };

    timer1_init();

    return &global;
};

// ****** USART BUFFER METHODS ******

void buffer_reset(sys_buffer *buf) {
    if (!buf) return;

    memset(buf->buffer, 0, sizeof(buf->buffer));
    buf->first = 0;
    buf->last = 0;
}

void buffer_sendch(sys_buffer *buf, char ch) {
    if (!buf) return;

    if (buf->last >= BUFFER_SIZE) {
        buffer_reset(buf);
    }

    buf->buffer[buf->last] = ch;
    buf->last++;
}

void buffer_readch(sys_buffer *buf, char *ch) {
    if (!buf) return;
    if (!ch) return;

    if (buf->first == 0 && buf->last == 0) return;
    else if (buf->first == buf->last) {
        buffer_reset(buf);
        return;
    }

    *ch = buf->buffer[buf->first];
    buf->first++;
}

// ****** TIMER 1 ******

void timer1_init(void) {
    TCCR1B = (1 << CS11) | (1 << CS10);
    TCNT1 = 0;
}

static inline int16_t _uget_ch_timeout(uint16_t time_tick) {
    uint16_t start = TCNT1;
    uint16_t now = 0, elapsed = 0;

    while (true) {
        now = TCNT1;

        if (UCSR0A & (1 << RXC0)) return UDR0;

        if (now >= start) elapsed = now - start;
        else elapsed = (65536 - start) + now;

        if (elapsed >= time_tick) return 0;
    }
}

// ****** USART SCAN CHAR ******

static inline char _uget_ch_nobuf(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

int16_t uset_ch(sys_buffer *buf, char *copy) {
    if (!buf) return -1;

    static char next = 0;
    if (next != 0) {
        if (copy) *copy = next;

        buffer_sendch(buf, next);
        return buf->last;
    }
    
    char ch = _uget_ch_nobuf();

    if (ch == '\n') ch = '\0';
    else if (ch == '\r') {
        next = _uget_ch_timeout(1000);
        if (next == '\n') next = 0;

        ch = '\0';
    }

    if (copy) *copy = ch;

    buffer_sendch(buf, ch);
    return buf->last;
}

// ****** USART SCAN STR ******

int16_t uset_line(sys_buffer *buf, uint8_t len) {
    if (!buf) return -1;

    char ch = 0;
    uint8_t str_index = buf->last;

    for (uint8_t i = 0; i < len; i++) {
        uset_ch(buf, &ch);

        if (ch == '\n') break;
    }

    return str_index;
}

// ****** USART READ CHAR ******

int16_t read_ch(sys_buffer *buffer, char *ch) {
    if (!buffer) return -1;
    if (!ch) return -1;

    *ch = cbuffer_readch(buffer);

    return buffer->head;
}

// ****** USART READ STR ******

int16_t read_line_ends(sys_buffer *buffer, char *line_buf, uint8_t len) {
    if (!buffer) return -1;
    if (!line_buf) return -1;

    char ch = 0, next = 0;
    uint8_t str_index = buffer->head;

    for (uint8_t i = 0; i < len; i++) {
        read_ch(buffer, &ch);

        if (ch == '\n') break;
        if (ch == '\r') {
            read_ch(buffer, &next);
            
            if (next == '\n') break;
            else next = 0;
        }

        line_buf[i] = ch;
        if (next != 0 && i != len - 1) {
            line_buf[i + 1] = ch;
        }
    }

    return str_index;
}

int16_t read_line(sys_buffer *buffer, char *line_buf, uint8_t len) {
    if (!buffer) return -1;
    if (!line_buf) return -1;

    char ch = 0;
    uint8_t str_index = buffer->head;

    for (uint8_t i = 0; i < len; i++) {
        read_ch(buffer, &ch);

        if (ch == '\n') return str_index;
        if (ch == '\r') {
            read_ch(buffer, &ch);
            
            if (ch == '\n') return str_index;
        }

        line_buf[i] = ch;
    }

    return str_index;
}