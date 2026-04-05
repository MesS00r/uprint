#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <usart.h>

int main(void) {
    ubegin(9600);

    uprint0("hello ");
    uprintln0("uprint!!!");

    uput_ch('\n');

    uprint("NUMS -> {%d, %b, %x} ", 234, 234, 234);
    uprintln("END_NUM: %d", 0);

    uput_ch('\n');

    uprint_dec(123); uput_ch('\n');
    uprint_bin(123); uput_ch('\n');
    uprint_hex(123); uput_ch('\n');

    uput_str("\nend code.\n");

    return 0;
}