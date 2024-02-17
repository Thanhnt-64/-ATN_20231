#include "contiki.h"
#include "uart-arch.h"
#include "slip.h"

void slip_arch_init(){
    uart_set_input(slip_input_byte);
}

void slip_arch_writeb(unsigned char c)
{
    uint8_t len = 1;
    uart_write(&c, len);
}