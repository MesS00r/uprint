#ifndef USARTINPUT_HPP_INCLUDED
#define USARTINPUT_HPP_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <string.h> // IWYU pragma: keep

#define BUFFER_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sys_buffer sys_buffer;

sys_buffer* buffer_init(void);
void buffer_reset(sys_buffer *buffer);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //USARTINPUT_HPP_INCLUDED