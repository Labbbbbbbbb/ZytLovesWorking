#ifndef UART_RECEIVE
#define UART_RECEIVE

#include "stm32f1xx.h"
#include "usart.h"

extern uint16_t spe1;
extern uint16_t spe2;

extern int16_t target_spe1;
extern int16_t target_spe2;
extern uint8_t Receivebuffer[8];

#endif // !UART_RECEIVE