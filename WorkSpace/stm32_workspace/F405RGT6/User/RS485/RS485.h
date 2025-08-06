#ifndef RS485_H
#define  RS485_H


#include "usart.h"

#define EN_PORT RS485_RE_GPIO_Port
#define EN_PIN RS485_RE_Pin
#define huart_485 &huart3
#define UART_485_Instance USART3

extern uint8_t RS485_TxData[8];
extern uint8_t RS485_RxData[8];
extern uint8_t RS485_RxBuffer[12];

void RS485_Init(void);
void RS485_Transmit(uint8_t* TxData);
void RS485_Receive_Decode(uint8_t* RxData);

#endif // !1