#include "types.h"
void uart0_init(u32 baud);
void uart0_tx(u8 byte);
u8 uart0_rx(void);
void uart0_tx_integer(s32 num);
