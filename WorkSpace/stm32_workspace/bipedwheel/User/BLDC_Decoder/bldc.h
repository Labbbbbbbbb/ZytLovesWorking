#ifndef BLDC_H
#define BLDC_H
#include "stdint.h"
#include "stm32f407xx.h"
#include "usart.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#define BLDC_UART &huart2

extern uint8_t bldc_rxdata[1];
extern float bldc_msg[4];

void parse_uart_data(void) ;
#endif