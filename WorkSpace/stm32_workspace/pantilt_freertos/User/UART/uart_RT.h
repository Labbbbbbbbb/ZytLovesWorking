#ifndef UART_RECEIVE
#define UART_RECEIVE

#include "stm32f1xx.h"
#include "usart.h"


extern uint8_t ReceivebufferG[6];
extern uint8_t ReceivebufferR[14];
void U_Transmit(uint8_t num);

#endif // !UART_RECEIVE